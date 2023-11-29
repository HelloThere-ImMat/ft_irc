/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/28 17:39:45 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::cap(const std::vector<std::string> &cmd, Client *const client) {
	static const std::string capCommand = "CAP";
	
	if (cmd[0] == capCommand)
	{
		client->addToLoginMask(CAP_LOGIN);
		std::cout << "CAP added to mask" << std::endl;
	}
}

void Server::pass(const std::vector<std::string> &cmd, Client *const client) {
	static const std::string passCommand = "PASS";

	if (cmd[0] == passCommand) {
		if (cmd[1] == _password) {
			std::cout << "Valid Password!" << std::endl;
			client->addToLoginMask(PASS_LOGIN);
		} else {throw InvalidPasswordException();}
	}
}

void Server::user(const std::vector<std::string> &cmd, Client *const client) {
	static const std::string userCommand = "USER";

	if (cmd[0] == userCommand)
	{
		client->setUsername(cmd[1]);
		client->addToLoginMask(USER_LOGIN);
		std::cout << "USER added to mask" << std::endl;
	}
}

void Server::nick(const std::vector<std::string> &cmd, Client *const client) {
	static const std::string nickCommand = "NICK";

	if (cmd[0] == nickCommand)
	{
		client->setNickname(cmd[1]);
		client->addToLoginMask(NICK_LOGIN);
		std::cout << "NICK added to mask" << std::endl;
	}
}

void Server::ping(const std::vector<std::string> &cmd, Client *const client) {
	static const std::string pingCommand = "PING";

	if (cmd[0] == pingCommand)
		sendMessage(":127.0.0.1 PONG 127.0.0.1 :" + client->getUsername(), client->getSocketFd());
}
