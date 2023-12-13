/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:09:20 by mat               #+#    #+#             */
/*   Updated: 2023/12/13 11:29:11 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

enum kickError {
	WRONG_CHAN_NAME,
	WRONG_USER_NAME,
	USER_NOT_IN_CHAN,
	USER_NOT_OP,
	TARGET_NOT_IN_CHAN
};

static void handleError(const int errorCode, Client *const client,
		const std::vector<std::string> &cmd) {
	switch (errorCode) {
		case WRONG_CHAN_NAME:
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
			break;
		case WRONG_USER_NAME:
			Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
			break;
		case USER_NOT_IN_CHAN:
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, cmd[1]);
			break;
		case USER_NOT_OP:
			Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, cmd[1]);
			break;
		case TARGET_NOT_IN_CHAN:
			Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client, cmd[1]);
			break;
	}
}

void Server::kick(const std::vector<std::string> &cmd, Client *const client) {
	const size_t cmdSize = cmd.size();
	if (cmdSize < 3) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	};
	std::map<std::string, Channel *>::iterator itMap = _channels.find(cmd[1]);
	try {
		if (itMap != _channels.end())
			throw (OpCmdsErrors(WRONG_CHAN_NAME));
		Channel						   *channel = itMap->second;
		const std::vector<std::string> users =
			Utils::splitString(cmd[2], CMD_ARG_SEPARATOR);
		for (std::vector<std::string>::const_iterator itUser = users.begin();
			itUser != users.end(); itUser++) {
			Client * kickedUser = _clientMap.getClient(*itUser);
			client->setLastArg((*itUser));
			if (kickedUser == NULL)
				throw(OpCmdsErrors(WRONG_USER_NAME));
			if (channel->isUserInChannel(client) == false)
				throw(OpCmdsErrors(USER_NOT_IN_CHAN));
			if (channel->isOp(client) == false)
				throw(OpCmdsErrors(USER_NOT_OP));
			if (channel->isUserInChannel(kickedUser) == false)
				throw(OpCmdsErrors(TARGET_NOT_IN_CHAN));
			std::string kickMessage =
				Utils::getFormattedMessage(
					KICK, client, cmd[1]);
			if (cmdSize > 3)
				kickMessage += Utils::getFullMessage(cmd, 3);
			else
				kickMessage += client->getNickname();
			channel->sendToAll(client, kickMessage);
		}
	} catch (Server::OpCmdsErrors &e) {
		handleError(e.getCode(), client, cmd);
	}
}
