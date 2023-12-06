/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 15:34:54 by rbroque          ###   ########.fr       */
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

static std::string getFullMessage(
	const std::vector<std::string> &cmd, const size_t startIndex) {
	std::string	 fullMessage;
	const size_t size = cmd.size();
	size_t		 i = startIndex;
	if (size > startIndex) {
		while (i < size) {
			if (i > startIndex)
				fullMessage += MESSAGE_SEPARATOR;
			fullMessage += cmd[i++];
		}
	}
	return (fullMessage);
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
		SendCmd::sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else if (cmd.size() < 2)
		SendCmd::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
	else if (cmd[1] == _password) {
		client->addToLoginMask(PASS_LOGIN);
	} else {
		SendCmd::sendFormattedMessage(ERR_PASSWDMISMATCH, client);
		error(ERR_CLOSECONNECTION, client);
	}
}

void Server::user(const std::vector<std::string> &cmd, Client *const client) {
	if (client->getLogMask() & USER_LOGIN)
		SendCmd::sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else {
		if (cmd.size() < 2 || cmd[1].empty()) {
			client->setUsername(DEFAULT_USERNAME);
			SendCmd::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
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
		SendCmd::sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
	else if (isNicknameValid(cmd[1]) == false)
		SendCmd::sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
	else if (isNicknameAlreadyUsed(cmd[1]))
		SendCmd::sendFormattedMessage(ERR_NICKNAMEINUSE, client);
	else {
		client->setNickname(cmd[1]);
		_clientMap.updateClientNickname(client, cmd[1]);
		client->addToLoginMask(NICK_LOGIN);
	}
}

void Server::ping(const std::vector<std::string> &cmd, Client *const client) {
	(void)cmd;
	SendCmd::sendFormattedMessage(PONG_MESSAGE, client);
}

void Server::sendJoinMessage(const Channel *const channel, const Client *client,
	const std::string &channelName) {
	const std::string channelUserList = channel->getUserList();
	const std::string topic = channel->getTopic();

	if (!topic.empty())
		channel->sendTopic(client);
	channel->sendToAll(client, JOIN_PREFIX + channelName);
	SendCmd::sendFormattedMessage(UL_JOIN_MESSAGE + channelUserList, client);
	SendCmd::sendFormattedMessage(EUL_JOIN_MESSAGE, client);
}

void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	if (!cmd[1].empty()) {
		const std::map<std::string, Channel *>::iterator it =
			_channels.find(cmd[1]);
		if (it == _channels.end()) {
			Channel *channel = new Channel(cmd[1], client);
			_channels[cmd[1]] = channel;
			sendJoinMessage(channel, client, cmd[1]);
		} else {
			it->second->addNewUser(client);
			sendJoinMessage(it->second, client, cmd[1]);
		}
	}
}

void Server::privmsg(
	const std::vector<std::string> &cmd, Client *const client) {
	const std::string fullMessage = getFullMessage(cmd, PRIVMSG_START_INDEX);
	const std::string privMessage = PRIVMSG_PREFIX + cmd[1] + " " + fullMessage;
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);

	if (it != _channels.end()) {
		Channel *const channel = it->second;
		if (channel->isUserInChannel(client))
			channel->sendToOthers(client, privMessage);
	} else if (_clientMap.getClient(cmd[1]) != NULL)
		SendCmd::sendPrivateMessage(
			privMessage, client, _clientMap.getClient(cmd[1]));
	else
		printLog("Cound not send message");
}

void Server::part(const std::vector<std::string> &cmd, Client *const client) {
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);
	if (it != _channels.end()) {
		Channel *channel = it->second;
		channel->sendToAll(client, PART_PREFIX + cmd[1]);
		channel->removeUser(client);
	} else
		printLog("Could not part channel");
}

static std::string removeSetterChar(const std::string &message) {
	std::string newMessage;

	newMessage = message;
	if (message.empty() == false && message[0] == SETTER_CHAR)
		newMessage.erase(0, 1);
	return newMessage;
}

void Server::topic(const std::vector<std::string> &cmd, Client *const client) {
	const size_t size = cmd.size();

	if (size < 2) {
		SendCmd::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);
	if (it != _channels.end()) {
		Channel *const channel = it->second;
		if (channel->isUserInChannel(client) == false)
			SendCmd::sendFormattedMessage(ERR_NOTONCHANNEL, client);
		else if (size == 2)
			channel->sendTopic(client);
		else if (channel->canChangeTopic(client) == false) {
			SendCmd::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client);
		} else {
			const std::string topic =
				removeSetterChar(getFullMessage(cmd, TOPIC_START_INDEX));
			channel->setTopic(topic);
			channel->sendTopicToAll(client);
		}
	} else {
		SendCmd::sendFormattedMessage(ERR_NOSUCHCHANNEL, client);
	}
}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	SendCmd::sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	printLog(CLOSED_CLIENT_MESSAGE);
}
