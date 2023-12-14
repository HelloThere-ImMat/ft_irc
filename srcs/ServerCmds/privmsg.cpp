/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:47:09 by mat               #+#    #+#             */
/*   Updated: 2023/12/14 13:49:40 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////////////
// Static methods //
///////////////////

static bool areSameConv(const Conversation &conv1, const Conversation &conv2){
	return (conv1.user1 == conv2.user1 && conv1.user2 == conv2.user2)
		|| (conv1.user1 == conv2.user2 && conv1.user2 == conv2.user1);
}

static bool isNewConv(const std::vector<Conversation> &convMap, const Conversation &conv) {
	for (std::vector<Conversation>::const_iterator it = convMap.begin(); it != convMap.end(); ++it) {
		if (areSameConv(*it, conv))
			return false;
	}
	return true;
}

//////////////////////
// Private methods //
/////////////////////

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

void Server::sendPrivmsgToUser(Client *const client,
	const std::string &targetName, const std::string &privMessage) {
	const Client *const receiver = _clientMap.getClient(targetName);
	if (receiver == NULL) {
		client->setLastArg(targetName);
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
	} else {
		const Conversation conv = {.user1 = client, .user2 = receiver};
		if (isNewConv(_convMap, conv))
			_convMap.push_back(conv);
		Utils::sendPrivateMessage(privMessage, client, receiver);
	}
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
		else {
			sendPrivmsgToUser(client, *itTarget, privMessage);
		}
	}
	// std::cout << "Conv Map:" << std::endl;
	// for (std::vector<Conversation>::iterator it = _convMap.begin(); it != _convMap.end(); ++it) {
	// 	std::cout << "User1:"<< it->user1->getNickname() << "; User2:" << it->user2->getNickname() << std::endl;
	// }
}
