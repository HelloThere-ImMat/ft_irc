/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 09:24:03 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Static

std::string getSpecifiedNick(const SpecifiedClient &spClient)
{
	if (spClient.isOp)
		return (OP_PREFIX + spClient.client->getNickname());
	return (spClient.client->getNickname());
}

// Methods

Channel::Channel(const std::string name, const Client *const client) : _name(name)
{
	const SpecifiedClient spClient = {.client = client, .isOp = true};

	userMap[client->getNickname()] = spClient;
}

Channel::~Channel() { userMap.clear(); }

void Channel::addNewUser(const Client *const client)
{
	SpecifiedClient spClient = {.client = client, .isOp = false};

	userMap[client->getNickname()] = spClient;
}

const std::string Channel::getUserList() const
{
	std::string userList;
	for (std::map<std::string, SpecifiedClient>::const_iterator it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it != userMap.begin())
			userList += " ";
		userList += getSpecifiedNick(it->second);
	}
	return (userList);
}

void Channel::removeUser(const Client *const client)
{
	std::map<std::string, SpecifiedClient>::iterator it;
	std::string nickname = client->getNickname();
	userMap.erase(nickname);
}

const std::string &Channel::getName() const
{
	return _name;
}


void Channel::sendToChannel(const Client *const client, std::string message, bool sendToSelf) const
{
	if (sendToSelf)
		SendCmd::sendPrivateMessage(message, client, client);
	for (std::map<std::string, SpecifiedClient>::const_iterator it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it->second.client->getSocketFd() != client->getSocketFd())
			SendCmd::sendPrivateMessage(message, client, it->second.client);
	}
}

bool Channel::userIsInChannel(const Client *const client)
{
	std::string nick = client->getNickname();
	for (std::map<std::string, SpecifiedClient>::iterator it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it->second.client->getNickname() == nick)
			return (true);
	}
	return (false);
}
