/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 01:52:37 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client *const client, const std::string &username)
{
	_name = name;
	_userMap[username] = new SpecifiedClient(client);
	std::cout << "Successfully created a Channel :" << username << std::endl;
}


void Channel::addNewUser(Client *const client)
{
	_userMap[client->getUsername()] = new SpecifiedClient(client);
}

void Channel::sendToChannel(std::string &username)
{
	(void)username;
}
