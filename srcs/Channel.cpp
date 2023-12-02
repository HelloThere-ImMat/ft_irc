/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/02 20:07:16 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client *const client)
{
	_name = name;
	userFds.push_back(client->getSocketFd());
	userNames.push_back("@" + client->getNickname());
	std::cout << "Successfully created a Channel" << std::endl;
}


void Channel::addNewUser(Client *const client)
{
	userFds.push_back(client->getSocketFd());
	userNames.push_back(client->getNickname());
}

void Channel::removeUser(Client *const client)
{
	int userFd = client->getSocketFd();
	std::string nickname = client->getNickname();
	for (std::vector<int>::iterator it = userFds.begin(); it != userFds.end(); it++)
	{
		if (*it == userFd)
			userFds.erase(it);
	}
	//remove from usernames aswell
}

const std::string &Channel::getName() const
{
	return _name;
}
