/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 08:14:40 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
}

void Server::nick(const std::vector<std::string> &cmd, Client *const client) {
	client->setNickname(cmd[1]);  // Check if argument exists
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