/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 09:46:56 by mat              ###   ########.fr       */
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

static std::string getFullMessage(const std::vector<std::string> &cmd)
{
	std::string fullMessage;
	int size = cmd.size();
	int i = PRIVMSG_START_INDEX;
	if (size > 2)
	{
		while (i < size)
		{
			if (i > PRIVMSG_START_INDEX)
				fullMessage += " ";
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

void Server::privmsg(
	const std::vector<std::string> &cmd, Client *const client) {
	std::string fullMessage = getFullMessage(cmd);

	if (_channels.find(CHANNEL_PREFIX + cmd[1]) != _channels.end())
	{
		Channel *channel  = _channels.find(CHANNEL_PREFIX + cmd[1])->second;
		if (channel->userIsInChannel(client))
			channel->sendToChannel(client, PRIVMSG_PREFIX + cmd[1] + " " + fullMessage, false);
	}
	else if (_clientMap.getClient(cmd[1]) != NULL)
		SendCmd::sendPrivateMessage(PRIVMSG_PREFIX + cmd[1] + " " + fullMessage, client, _clientMap.getClient(cmd[1]));
	else
		printLog("Cound not send message");
}

void Server::part(const std::vector<std::string> &cmd, Client *const client)
{
	if (_channels.find(CHANNEL_PREFIX + cmd[1]) !=	_channels.end()) {
		Channel *channel = _channels.find(CHANNEL_PREFIX + cmd[1])->second;
		channel->sendToChannel(client, PART_PREFIX + cmd[1], true);
		channel->removeUser(client);
	} else
		printLog("Could not part channel");
}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	SendCmd::sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	printLog(CLOSED_CLIENT_MESSAGE);
}
