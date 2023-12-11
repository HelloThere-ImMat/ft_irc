/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:17:28 by mat               #+#    #+#             */
/*   Updated: 2023/12/11 14:48:20 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::quit(const std::vector<std::string> &cmd, Client *const client)
{
	std::string	 quitMessage = Utils::getFullMessage(cmd, 0);
	for(std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		const Channel *const channel = it->second;
		if (channel->isUserInChannel(client))
			channel->sendToOthers(client, quitMessage);
	}
	error(QUIT_RESPONSE, client);
}
