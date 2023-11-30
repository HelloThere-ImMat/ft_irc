/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 12:33:59 by mat              ###   ########.fr       */
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
	std::vector<int> channelFds;

	if (_channels.find(cmd[1]) != _channels.end())
	{
		Channel *channel = _channels.find(cmd[1])->second;
		channelFds = channel->getUserfds();
		for (std::vector<int>::iterator it = channelFds.begin(); it != channelFds.end(); it++)
			std::cout << "sending a message on channel to " << *it << std::endl;
		std::cout << "Talks on a channel" << std::endl;
	}
	//else if test for nicknames
	else
		std::cout << "Talks to a user" << std::endl;

///////////////////// MSG SENT FORMAT /////////////////////////////////////
// query PRIVMSG
// >> :MATnb2!~mat@60ef-2fc4-d0c4-c934-68b4.abo.wanadoo.fr PRIVMSG MATnb1 :hi
// >> :<nickname sender>!~<hostname>@<address> PRIVMSG <nickname receiver> : <msg>

// channel PRIVMSG
// >> :MATnb2!~mat@60ef-2fc4-d0c4-c934-68b4.abo.wanadoo.fr PRIVMSG #testChannel :comment va
// >> :<nickname sender>!~<hostname>@<address> PRIVMSG <channel> : <msg>

}
