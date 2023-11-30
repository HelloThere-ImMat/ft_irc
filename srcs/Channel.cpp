/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 12:36:26 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client *const client, const std::string &nickname)
{
	_name = name;
	_userMap[nickname] = new SpecifiedClient(client, OPERATOR);
	std::cout << "Successfully created a Channel :" << nickname << std::endl;
}


void Channel::addNewUser(Client *const client)
{
	_userMap[client->getNickname()] = new SpecifiedClient(client, STANDARD);
}

void Channel::sendToChannel(std::string &nickname)
{
	(void)nickname;
}

std::vector<int> Channel::getUserfds()
{
	std::vector<int> channelFds;
	for (std::map<std::string, SpecifiedClient *>::iterator it = _userMap.begin(); it != _userMap.end(); it++)
		channelFds.push_back(it->second->getSocketFd());
	return (channelFds);
}
