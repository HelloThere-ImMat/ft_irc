/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:33 by mat               #+#    #+#             */
/*   Updated: 2023/12/14 14:50:48 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::part(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd.size() < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const std::vector<std::string> channels =
		Utils::splitString(cmd[1], CMD_ARG_SEPARATOR);
	for (std::vector<std::string>::const_iterator itCurrChanName =
			 channels.begin();
		 itCurrChanName != channels.end(); itCurrChanName++) {
		const std::map<std::string, Channel *>::iterator itMap =
			_channels.find(*itCurrChanName);
		if (itMap != _channels.end()) {
			Channel *const channel = itMap->second;
			if (channel->isUserInChannel(client)) {
				channel->sendToAll(client, PART_PREFIX + *itCurrChanName);
				client->rmFromChanMap(channel);
				channel->removeUser(client);
			} else
				Utils::sendFormattedMessage(
					ERR_NOTONCHANNEL, client, *itCurrChanName);
		} else
			Utils::sendFormattedMessage(
				ERR_NOSUCHCHANNEL, client, *itCurrChanName);
	}
}
