/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/04 14:52:56 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Static

std::string getSpecifiedNick(SpecifiedClient &spClient)
{
	if (spClient.isOp)
		return ("@" + spClient.client->getNickname());
	return (spClient.client->getNickname());
}

// Methods

Channel::Channel(std::string name, Client *const client)
{
	SpecifiedClient spClient = {.client = client, .isOp = true};

	_name = name;
	userMap[client->getNickname()] = spClient;
	std::cout << "Successfully created a Channel" << std::endl;
}

Channel::~Channel() {
	userMap.clear();
}

void Channel::addNewUser(Client *const client)
{
	SpecifiedClient spClient = {.client = client, .isOp = false};

	userMap[client->getNickname()] = spClient;
}

const std::string Channel::getUserList()
{
	std::string userList;
	for (std::map<std::string, SpecifiedClient>::iterator it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it == userMap.begin())
			userList = getSpecifiedNick(it->second);
		else
			userList += " " + getSpecifiedNick(it->second);
	}
	return (userList);
}

void Channel::removeUser(Client *const client)
{
	std::map<std::string, SpecifiedClient>::iterator it;
	std::string nickname = client->getNickname();
	userMap.erase(nickname);
}

const std::string &Channel::getName() const
{
	return _name;
}
