/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 09:23:08 by rbroque          ###   ########.fr       */
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
	if (cmd.size() < 2)
		sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
	else if (client->getLogMask() & PASS_LOGIN)
		sendFormattedMessage(ERR_ALREADYREGISTERED, client);
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
}

void Server::error(const std::string &message, Client *const client) {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	sendFormattedMessage(formatErrorMessage, client);
	closeClient(client);
	throw ClosedClientException();
}