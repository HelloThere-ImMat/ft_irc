/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:20:55 by mat               #+#    #+#             */
/*   Updated: 2023/12/10 01:28:25 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void handleError(
	const int errorCode, Client *const client, std::string args[2]) {
	switch (errorCode) {
		case 1:
			client->setLastArg(args[0]);
			Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
			break;
		case 2:
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, args[1]);
			break;
		case 3:
			Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, args[1]);
			break;
		case 4:
			Utils::sendFormattedMessage(ERR_USERONCHANNEL, client, args[1]);
			break;
	}
}

void Server::invite(const std::vector<std::string> &cmd, Client *const client) {
	const size_t cmdSize = cmd.size();
	if (cmdSize < 3) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const Client *const target = _clientMap.getClient(cmd[1]);
	try {
		if (target == NULL)
			throw(InviteErrors(1));
		const std::map<std::string, Channel *>::iterator it =
			_channels.find(cmd[2]);
		if (it == _channels.end())
			throw(InviteErrors(2));
		const Channel *const channel = it->second;
		const std::string	 channelName = cmd[2];
		if (channel->isUserInChannel(client) == false)
			throw(InviteErrors(3));
		if (channel->isUserInChannel(target) == false)
			throw(InviteErrors(4));
		// SHOULD reject it when the channel has invite-only
		// mode set, and the user is not a channel operator.
		client->setLastArg(cmd[1]);
		Utils::sendFormattedMessage(RPL_INVITING, client, channelName);
		const std::string invitation =
			Utils::getFormattedMessage(INVITATION, target, channelName);
		Utils::sendPrivateMessage(invitation, client, target);
	} catch (InviteErrors &e) {
		const int	errorType = e.getCode();
		std::string args[2] = {cmd[1], cmd[2]};
		handleError(errorType, client, args);
	}
}