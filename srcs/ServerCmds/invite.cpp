/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:20:55 by mat               #+#    #+#             */
/*   Updated: 2023/12/14 11:44:48 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

enum inviteError {
	WRONG_USER_NAME,
	WRONG_CHAN_NAME,
	USER_NOT_IN_CHAN,
	TARGET_IN_CHAN,
	USER_NOT_OP
};

static void handleError(const int errorCode, const Client *const client,
	const std::vector<std::string> &cmd) {
	switch (errorCode) {
		case WRONG_USER_NAME:
			Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
			break;
		case WRONG_CHAN_NAME:
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[2]);
			break;
		case USER_NOT_IN_CHAN:
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, cmd[2]);
			break;
		case TARGET_IN_CHAN:
			Utils::sendFormattedMessage(ERR_USERONCHANNEL, client, cmd[2]);
			break;
		case USER_NOT_OP:
			Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, cmd[2]);
			break;
	}
}

void Server::invite(const std::vector<std::string> &cmd, Client *const client) {
	const size_t cmdSize = cmd.size();
	if (cmdSize < 3) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	try {
		const std::string								 channelName = cmd[2];
		const std::map<std::string, Channel *>::iterator it =
			_channels.find(channelName);
		if (it == _channels.end())
			throw(OpCmdsErrors(WRONG_CHAN_NAME));
		Channel *const channel = it->second;
		if (channel->isUserInChannel(client) == false)
			throw(OpCmdsErrors(USER_NOT_IN_CHAN));
		if (channel->canInvite(client) == false)
			throw(OpCmdsErrors(USER_NOT_OP));
		const Client *const target = _clientMap.getClient(cmd[1]);
		if (target == NULL)
			throw(OpCmdsErrors(WRONG_USER_NAME));
		if (channel->isUserInChannel(target) == true)
			throw(OpCmdsErrors(TARGET_IN_CHAN));
		if (channel->isInInviteList(target) == false)
			channel->addToInviteList(target);
		Utils::sendFormattedMessage(RPL_INVITING, client, channelName);
		const std::string invitation =
			Utils::getFormattedMessage(INVITATION, target, channelName);
		Utils::sendPrivateMessage(invitation, client, target);
	} catch (Server::OpCmdsErrors &e) {
		handleError(e.getCode(), client, cmd);
	}
}
