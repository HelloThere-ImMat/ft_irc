/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/04 11:39:36 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client *const client)
{
	_name = name;
	userFds.push_back(client->getSocketFd());
	_operatorFd = client->getSocketFd();
	userNicks.push_back("@" + client->getNickname());
	std::cout << "Successfully created a Channel" << std::endl;
}

Channel::~Channel() {
	// _userNicks.clear();
}

void Channel::addNewUser(Client *const client)
{
	userFds.push_back(client->getSocketFd());
	userNicks.push_back(client->getNickname());
	// mettre les deux vectors en un (std::vector<Client *>)
}

void Channel::removeUser(Client *const client)
{
	int userFd = client->getSocketFd();
	std::string nickname = client->getNickname();
	for (std::vector<int>::iterator it = userFds.begin(); it != userFds.end(); it++) {
		if (*it == userFd)
			userFds.erase(it);
	}	
	for (std::vector<std::string>::iterator it = userNicks.begin(); it != userNicks.end(); it++) {
		if (*it == nickname || *it == "@" + nickname)
			userNicks.erase(it);
	}
}

const std::string &Channel::getName() const
{
	return _name;
}
