/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/09 23:35:38 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// STATIC //
////////////

static bool isAuthorizedChar(const char c) {
	static const std::string specialAuthorizedSet = SPECIAL_NICK_CHARSET;

	return isalnum(c) || specialAuthorizedSet.find(c) != std::string::npos;
}

static bool areOnlyAuthorizedChar(const std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (isAuthorizedChar(str[i]) == false)
			return false;
	}
	return true;
}

static bool isNicknameValid(const std::string &nickname) {
	return isdigit(nickname[0]) == false && areOnlyAuthorizedChar(nickname);
}

static std::string removeSetterChar(const std::string &message) {
	std::string newMessage;

	newMessage = message;
	if (message.empty() == false && message[0] == SETTER_CHAR)
		newMessage.erase(0, 1);
	return newMessage;
}

static void removeDuplicateChars(std::string &str) {
	bool charSet[256] = {false};  // Assuming ASCII characters

	size_t currentIndex = 0;
	size_t len = str.length();

	for (size_t i = 0; i < len; ++i) {
		char currentChar = str[i];
		if (!charSet[static_cast<unsigned char>(currentChar)]) {
			charSet[static_cast<unsigned char>(currentChar)] = true;
			str[currentIndex++] = currentChar;
		}
	}
	str.resize(currentIndex);
}

static void keepOnlySpecificChars(
	std::string &str, const std::string &allowedChars) {
	for (std::string::iterator it = str.begin(); it != str.end();) {
		if (allowedChars.find(*it) == std::string::npos) {
			it = str.erase(it);
		} else {
			++it;
		}
	}
}

static void setModeStr(std::string &str) {
	keepOnlySpecificChars(str, "itklo+-");
	removeDuplicateChars(str);
	if (str.empty() == false) {
		char last = str[str.length() - 1];
		if (last == '-' || last == '+')
			str.erase(str.length() - 1);
	}
}

/////////////
// PRIVATE //
/////////////

void Server::cap(const std::vector<std::string> &cmd, Client *const client) {
	(void)cmd;
	client->addToLoginMask(CAP_LOGIN);
}

void Server::pass(const std::vector<std::string> &cmd, Client *const client) {
	if (client->getLogMask() & PASS_LOGIN)
		Utils::sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else if (cmd.size() < 2)
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
	else if (cmd[1] == _password) {
		client->addToLoginMask(PASS_LOGIN);
	} else {
		Utils::sendFormattedMessage(ERR_PASSWDMISMATCH, client);
		error(ERR_CLOSECONNECTION, client);
	}
}

void Server::user(const std::vector<std::string> &cmd, Client *const client) {
	if (client->getLogMask() & USER_LOGIN)
		Utils::sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else {
		if (cmd.size() < 2 || cmd[1].empty()) {
			client->setUsername(DEFAULT_USERNAME);
			Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		} else if (cmd[1].length() > USERLEN) {
			client->setUsername(cmd[1].substr(0, USERLEN));
		} else
			client->setUsername(cmd[1]);
		client->addToLoginMask(USER_LOGIN);
	}
}

bool Server::isNicknameAlreadyUsed(const std::string &nickname) {
	return _clientMap.getClient(nickname) != NULL;
}

void Server::nick(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd[1].empty())
		Utils::sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
	else if (isNicknameValid(cmd[1]) == false)
		Utils::sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
	else if (isNicknameAlreadyUsed(cmd[1]))
		Utils::sendFormattedMessage(ERR_NICKNAMEINUSE, client);
	else {
		client->setNickname(cmd[1]);
		_clientMap.updateClientNickname(client, cmd[1]);
		client->addToLoginMask(NICK_LOGIN);
	}
}

void Server::ping(const std::vector<std::string> &cmd, Client *const client) {
	(void)cmd;
	Utils::sendFormattedMessage(PONG_MESSAGE, client);
}

void Server::topic(const std::vector<std::string> &cmd, Client *const client) {
	const size_t size = cmd.size();

	if (size < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);
	if (it != _channels.end()) {
		Channel *const channel = it->second;
		if (channel->isUserInChannel(client) == false)
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, cmd[1]);
		else if (size == 2)
			channel->sendTopic(client);
		else if (channel->canChangeTopic(client) == false) {
			Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, cmd[1]);
		} else {
			const std::string topic =
				removeSetterChar(Utils::getFullMessage(cmd, TOPIC_START_INDEX));
			channel->setTopic(topic);
			channel->sendTopicToAll(client);
		}
	} else {
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
	}
}

void Server::mode(
	const std::vector<std::string> &cmdVector, Client *const client) {
	std::vector<std::string> cmd = cmdVector;
	const size_t			 size = cmd.size();

	if (size < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	} else if (cmd[1][0] != CHANNEL_PREFIX)
		return;
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);
	if (it != _channels.end()) {
		Channel *const channel = it->second;
		if (size == 2) {
			channel->sendMode(client);
		} else if (channel->isOp(client) == false) {
			Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client);
		} else {
			setModeStr(cmd[2]);
			if (channel->processMode(cmd, client))
				channel->sendToAll(client, Utils::getFullMessage(cmd, 0));
		}
	} else {
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client);
	}
}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	Utils::sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	printLog(CLOSED_CLIENT_MESSAGE);
}
