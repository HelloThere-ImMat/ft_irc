/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:33 by mat               #+#    #+#             */
/*   Updated: 2023/12/08 12:09:20 by mat              ###   ########.fr       */
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
		const std::map<std::string, Channel *>::iterator itM =
			_channels.find(*itCurrChanName);
		if (itM != _channels.end()) {
			Channel *const channel = itM->second;
			if (channel->isUserInChannel(client)) {
				channel->sendToAll(client, PART_PREFIX + *itCurrChanName);
				channel->removeUser(client);
			} else
				Utils::sendFormattedMessage(
					ERR_NOTONCHANNEL, client, *itCurrChanName);
		} else
			Utils::sendFormattedMessage(
				ERR_NOSUCHCHANNEL, client, *itCurrChanName);
	}
}
