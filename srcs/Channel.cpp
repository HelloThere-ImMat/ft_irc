/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 14:57:18 by mat              ###   ########.fr       */
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
