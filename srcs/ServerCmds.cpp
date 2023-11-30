/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 18:05:32 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// STATIC //
////////////

static bool containsCharactersFromSet(const std::string &str,
									  const std::string &charSet) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (charSet.find(str[i]) != std::string::npos) {
			return true;
		}
	}
	return false;
}

static bool isNicknameValid(const std::string &nickname) {
	return isdigit(nickname[0]) == false &&
		   containsCharactersFromSet(nickname, INVALID_NICK_CHARSET) == false;
}

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
	client->setUsername(cmd[1]);  // Check if argument exists
	client->addToLoginMask(USER_LOGIN);
}

void Server::nick(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd[1].empty())
		sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
	else if (isNicknameValid(cmd[1]) == false)
		sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
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