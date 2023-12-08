/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:33 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 17:42:02 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::part(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd.size() < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	std::vector<std::string> channels =
		Utils::splitString(cmd[1], CMD_ARG_SEPARATOR);
	for (std::vector<std::string>::iterator itV = channels.begin();
		 itV != channels.end(); itV++) {
		const std::map<std::string, Channel *>::iterator itM =
			_channels.find(*itV);
		if (itM != _channels.end()) {
			Channel *const channel = itM->second;
			if (!channel->isUserInChannel(client))
				Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, *itV);
			else {
				channel->sendToAll(client, PART_PREFIX + *itV);
				channel->removeUser(client);
			}
		} else
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
	}
}
