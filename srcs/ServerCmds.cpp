/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/04 14:43:00 by mat              ###   ########.fr       */
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
	int i = 3;
	if (size > 2)
	{
		fullMessage = cmd[2];
		while (i < size)
			fullMessage += " " + cmd[i++];
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
		sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else if (cmd.size() < 2)
		sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
	else if (cmd[1] == _password) {
		client->addToLoginMask(PASS_LOGIN);
	} else {
		sendFormattedMessage(ERR_PASSWDMISMATCH, client);
		error(ERR_CLOSECONNECTION, client);
	}
}

void Server::user(const std::vector<std::string> &cmd, Client *const client) {
	if (client->getLogMask() & USER_LOGIN)
		sendFormattedMessage(ERR_ALREADYREGISTERED, client);
	else {
		if (cmd.size() < 2 || cmd[1].empty()) {
			client->setUsername(DEFAULT_USERNAME);
			sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
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
		sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
	else if (isNicknameValid(cmd[1]) == false)
		sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
	else if (isNicknameAlreadyUsed(cmd[1]))
		sendFormattedMessage(ERR_NICKNAMEINUSE, client);
	else {
		client->setNickname(cmd[1]);
		_clientMap.updateClientNickname(client, cmd[1]);
		client->addToLoginMask(NICK_LOGIN);
	}
}

void Server::ping(const std::vector<std::string> &cmd, Client *const client) {
	(void)cmd;
	sendFormattedMessage(PONG_MESSAGE, client);
	_clientMap.printUserList();
}

void Server::sendJoinMessage(Channel *channel, Client *client, std::string channelName)
{
	std::string channelUserList;

	printLog(client->getNickname());
	sendPrivateMessage(JOIN_MESSAGE + channelName, client, client);
	for (std::map<std::string, SpecifiedClient>::iterator it = channel->userMap.begin(); it != channel->userMap.end(); it++) {
		if (it->second.client->getSocketFd() != client->getSocketFd())
			sendPrivateMessage(JOIN_MESSAGE + channelName, client, it->second.client);
	}
	if (!channel->topic.empty())
		sendFormattedMessage(TOPIC_JOIN_MESSAGE + channel->topic, client);
	channelUserList = channel->getUserList();
	sendFormattedMessage(UL_JOIN_MESSAGE + channelUserList, client);
	sendFormattedMessage(EUL_JOIN_MESSAGE, client);
}

void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	if (!cmd[1].empty())
	{
		std::map<std::string, Channel *>::iterator it = _channels.find("#" + cmd[1]);
		if (it == _channels.end())
		{
			Channel *channel = new Channel(cmd[1], client);
			_channels["#" + cmd[1]] = channel; 
			printLog("New Channel !");
			sendJoinMessage(channel, client, cmd[1]);
		}
		else
		{
			it->second->addNewUser(client);
			printLog("Join Channel !");
			sendJoinMessage(it->second, client, cmd[1]);
		}
	}
}

void Server::privmsg(const std::vector<std::string> &cmd, Client *const client)
{
	std::string senderNickname = client->getNickname();
	std::string fullMessage = getFullMessage(cmd);

	printLog("1-> Searching for :" + cmd[1] + " channel");
	if (_channels.find("#" + cmd[1]) != _channels.end())
	{
		Channel *channel  = _channels.find("#" + cmd[1])->second;
		printLog("2 -> Found a channel :" + channel->getName());
		for (std::map<std::string, SpecifiedClient>::iterator it = channel->userMap.begin(); it != channel->userMap.end(); it++)
		{
			if (it->second.client->getSocketFd() != client->getSocketFd())
			{
				printLog("3 -> Sent channel message to :" + it->second.client->getNickname());
				sendPrivateMessage(PRIVMSG_PREFIX + cmd[1] + " " + fullMessage, client, it->second.client);
			}
		}
	}
	else if (_clientMap.getClient(cmd[1]) != NULL)
		sendPrivateMessage(PRIVMSG_PREFIX + cmd[1] + " " + fullMessage, client, _clientMap.getClient(cmd[1]));
	else
		printLog("Cound not send message");
}

void Server::part(const std::vector<std::string> &cmd, Client *const client)
{
	std::cout << cmd[1] << std::endl;
	if (_channels.find("#" + cmd[1]) !=	_channels.end()) {
		Channel *channel = _channels.find("#" + cmd[1])->second;
		for (std::map<std::string, SpecifiedClient>::iterator it = channel->userMap.begin(); it != channel->userMap.end(); it++) {
			sendPrivateMessage(PART_MESSAGE + cmd[1], client, it->second.client);
		}
		channel->removeUser(client);
	}
	else
		printLog("Could not part channel");

}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	printLog(CLOSED_CLIENT_MESSAGE);
}
