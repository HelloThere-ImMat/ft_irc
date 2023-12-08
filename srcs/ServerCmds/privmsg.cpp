/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:09 by mat               #+#    #+#             */
/*   Updated: 2023/12/08 15:40:24 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendPrivmsgToChannel(const Client *const client,
	const std::string &channelName, const std::string &privMessage) {
	const std::map<std::string, Channel *>::iterator itMap =
		_channels.find(channelName);
	if (itMap == _channels.end())
		Utils::sendFormattedMessage(ERR_CANNOTSENDTOCHAN, client, channelName);
	else {
		Channel *const channel = itMap->second;
		if (channel->isUserInChannel(client))
			channel->sendToOthers(client, privMessage);
		else
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, channelName);
	}
}

void Server::sendPrivmsgToUser(const Client *const client,
	const std::string &targetName, const std::string &privMessage) {
	const Client *const receiver = _clientMap.getClient(targetName);
	if (receiver == NULL)
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
	else
		Utils::sendPrivateMessage(privMessage, client, receiver);
}

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
	const std::vector<std::string> targetList =
		Utils::splitString(cmd[1], CMD_ARG_SEPARATOR);
	for (std::vector<std::string>::const_iterator itTarget = targetList.begin();
		 itTarget != targetList.end(); itTarget++) {
		const std::string privMessage =
			PRIVMSG_PREFIX + *itTarget + " " + fullMessage;

		if ((*itTarget)[0] == CHANNEL_PREFIX)
			sendPrivmsgToChannel(client, *itTarget, privMessage);
		else
			sendPrivmsgToUser(client, *itTarget, privMessage);
	}
}
