/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 15:11:04 by mat              ###   ########.fr       */
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
	sendMessage(":127.0.0.1 PONG 127.0.0.1 :" + client->getUsername(),
				client->getSocketFd());
}

void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	if (!cmd[1].empty())
	{
		std::map<std::string, Channel *>::iterator it = _channels.find(cmd[1]);
		if (it == _channels.end())
			_channels["#" + cmd[1]] = new Channel(cmd[1], client, client->getNickname());
		else
			it->second->addNewUser(client);
	}	
///////////////////// MSG SENT FORMAT /////////////////////////////////////
// >> :MATnb1!~mat@60ef-2fc4-d0c4-c934-68b4.abo.wanadoo.fr JOIN :#testChannel
// >> :lion.tx.us.dal.net 353 MATnb1 = #testChannel :@MATnb1 
// >> :lion.tx.us.dal.net 366 MATnb1 #testChannel :End of /NAMES list.
}

void Server::privmsg(const std::vector<std::string> &cmd, Client *const client)
{
	std::string senderNickname = client->getNickname();
	(void)cmd;

	if (_channels.find(cmd[1]) != _channels.end())
	{
		std::cout << "user: found channel" << std::endl;
		//Channel *channel = _channels.find(cmd[1])->second;
	//	for (std::vector<int>::iterator it = channelFds.begin(); it != channelFds.end(); it++)
	//		std::cout << "sending a message on channel to " << *it << std::endl;
	//	std::cout << "Talks on a channel" << std::endl;
	}
	//else if test for nicknames
		//std::cout << "Talks to a user" << std::endl;
	//else

///////////////////// MSG SENT FORMAT /////////////////////////////////////
// query PRIVMSG
// >> :MATnb2!~mat@60ef-2fc4-d0c4-c934-68b4.abo.wanadoo.fr PRIVMSG MATnb1 :hi
// >> :<nickname sender>!~<hostname>@<address> PRIVMSG <nickname receiver> : hi

// channel PRIVMSG
// >> :MATnb2!~mat@60ef-2fc4-d0c4-c934-68b4.abo.wanadoo.fr PRIVMSG #testChannel :comment va
// >> :<nickname sender>!~<hostname>@<address> PRIVMSG <channel> : <msg>
	sendFormattedMessage(PONG_MESSAGE, client);
}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	printLog(CLOSED_CLIENT_MESSAGE);
}
