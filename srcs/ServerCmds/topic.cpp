/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:35:46 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/13 13:30:02 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void sendTopicToUser(Channel *const channel, Client *const client,
	const std::vector<std::string> &cmd) {
	const std::string topic =
		Utils::removeSetterChar(Utils::getFullMessage(cmd, TOPIC_START_INDEX));
	channel->setTopic(topic);
	channel->sendTopicToAll(client);
}

void Server::topic(const std::vector<std::string> &cmd, Client *const client) {
	const size_t size = cmd.size();

	if (size < TOPIC_START_INDEX) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const std::map<std::string, Channel *>::iterator it =
		_channels.find(cmd[1]);
	if (it == _channels.end()) {
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
	} else {
		Channel *const channel = it->second;
		if (channel->isUserInChannel(client) == false)
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, cmd[1]);
		else if (size == TOPIC_START_INDEX)
			channel->sendTopic(client);
		else if (channel->canChangeTopic(client))
			sendTopicToUser(channel, client, cmd);
		else
			Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, cmd[1]);
	}
}
