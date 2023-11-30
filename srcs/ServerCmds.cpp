/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 01:55:01 by mat              ###   ########.fr       */
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
		throw InvalidPasswordException();
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
	sendMessage(":127.0.0.1 PONG 127.0.0.1 :" + client->getUsername(),
				client->getSocketFd());
}

void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	if (!cmd[1].empty())
	{
		std::map<std::string, Channel *>::iterator it = _channels.find(cmd[1]);
		if (it == _channels.end())
			_channels["#" + cmd[1]] = new Channel(cmd[1], client, client->getUsername());
		else
			it->second->addNewUser(client);
	}
}

void Server::privmsg(const std::vector<std::string> &cmd, Client *const client)
{
	(void)client;
	if (_channels.find(cmd[1]) != _channels.end())
		std::cout << "Talks on a channel" << std::endl;
	else
		std::cout << "Talks to a user" << std::endl;
	//Add a vector<string> -> user list to search here in a else if;
	// else
	//		do nothing
}
