/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:09 by mat               #+#    #+#             */
/*   Updated: 2023/12/08 10:03:31 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::privmsg(
	const std::vector<std::string> &cmd, Client *const client) {
	const size_t size = cmd.size();
	if (size < 3) {
		if (size < 2)
			Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		else
			Utils::sendFormattedMessage(ERR_NOTEXTTOSEND, client);
		return;
	}

	const std::string fullMessage =
		Utils::getFullMessage(cmd, PRIVMSG_START_INDEX);
	std::vector<std::string> receiverList =
		Utils::splitString(cmd[1], CMD_ARG_SEPARATOR);
	for (std::vector<std::string>::iterator itV = receiverList.begin();
		 itV != receiverList.end(); itV++) {
		const std::string privMessage =
			PRIVMSG_PREFIX + *itV + " " + fullMessage;

		if ((*itV)[0] == CHANNEL_PREFIX) {
			const std::map<std::string, Channel *>::iterator itM =
				_channels.find(*itV);
			if (itM == _channels.end())
				Utils::sendFormattedMessage(ERR_CANNOTSENDTOCHAN, client, *itV);
			else {
				Channel *const channel = itM->second;
				if (!channel->isUserInChannel(client))
					Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, *itV);
				else
					channel->sendToOthers(client, privMessage);
			}
		} else {
			const Client *const receiver = _clientMap.getClient(*itV);
			if (receiver == NULL)
				Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
			else
				Utils::sendPrivateMessage(privMessage, client, receiver);
		}
	}
}
