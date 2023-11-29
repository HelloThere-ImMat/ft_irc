/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/29 17:29:48 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::cap(const std::vector<std::string> &cmd, Client *const client) {
	(void)cmd;
	client->addToLoginMask(CAP_LOGIN);
}

void Server::pass(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd[1] == _password) {	// Check if argument exists
		client->addToLoginMask(PASS_LOGIN);
	} else {
		throw InvalidPasswordException();  // Change by sendFormattedMessage
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
