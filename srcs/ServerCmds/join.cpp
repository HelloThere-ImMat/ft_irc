/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:45:58 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 15:17:23 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Join Methods

void Server::createChannel(const Client *const client, const std::string &channelName) {
	if ((channelName)[0] != CHANNEL_PREFIX)
		Utils::sendFormattedMessage(
			ERR_BADCHANMASK, client, channelName);
	else if (_channels.size() >= MAX_CHANNEL_NB)
		Utils::sendFormattedMessage(
			ERR_TOOMANYCHANNELS, client, channelName);
	else {
		Channel *channel = new Channel(channelName, client);
		_channels[channelName] = channel;
		sendJoinMessage(channel, client, channelName);
	}
}

void Server::joinChannel(const std::vector<std::string> &cmd, const Client *const client, Channel *channel, size_t keyIndex)
{
	std::vector<std::string> keySubArgs;
	std::string channelName = channel->getName();
	size_t sizeCmd = cmd.size();

	if (sizeCmd > 2)
		keySubArgs = Utils::splitString(cmd[2], CMD_ARG_SEPARATOR);
	try {
			channel->addNewUser(client, keySubArgs, keyIndex);
			sendJoinMessage(channel, client, channel->getName());}
	catch (std::exception &e){
		std::cout << Utils::getFormattedMessage(e.what(), client, channelName) << std::endl;
		Utils::sendFormattedMessage(
			ERR_BADCHANNELKEY, client, channelName);}
}


void Server::sendJoinMessage(const Channel *const channel, const Client *client,
	const std::string &channelName) {
	std::string channelUserList;

	channel->sendToAll(client, JOIN_PREFIX + channelName);
	channelUserList = channel->getUserList();
	Utils::sendFormattedMessage(
		UL_JOIN_MESSAGE + channelUserList, client, channelName);
	Utils::sendFormattedMessage(EUL_JOIN_MESSAGE, client, channelName);
}


void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	std::vector<std::string> channelSubArgs;
	size_t					 sizeCmd = cmd.size();
	if (sizeCmd < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	} else {
		channelSubArgs = Utils::splitString(cmd[1], CMD_ARG_SEPARATOR);
		size_t channelNb = channelSubArgs.size();
		for (size_t i = 0; i < channelNb; i++) {
			std::string channelName = channelSubArgs[i];
			const std::map<std::string, Channel *>::iterator it2 =
				_channels.find(channelName);
			if (it2 == _channels.end()) {
				createChannel(client, channelName);
			} else {
				joinChannel(cmd, client, it2->second, i);
			}
		}
	}
}