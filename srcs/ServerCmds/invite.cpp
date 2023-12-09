/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:20:55 by mat               #+#    #+#             */
/*   Updated: 2023/12/09 02:26:20 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::invite(const std::vector<std::string> &cmd, Client *const client)
{
	const size_t cmdSize = cmd.size();
	if (cmdSize < 3)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	const Client *const target = _clientMap.getClient(cmd[1]);
	if (target != NULL) {
		const std::map<std::string, Channel *>::iterator it = _channels.find(cmd[2]);
		if (it != _channels.end())
		{
			const Channel *const channel = it->second;
			const std::string channelName = channel->getName();
			if (channel->isUserInChannel(client))
			{
				if (channel->isUserInChannel(target))
					Utils::sendFormattedMessage(ERR_USERONCHANNEL, client, channelName);
				else
				{
					// SHOULD reject it when the channel has invite-only 
					// mode set, and the user is not a channel operator.
					client->setLastArg(cmd[1]);
					Utils::sendFormattedMessage(RPL_INVITING, client, channelName);
					const std::string invitation = Utils::getFormattedMessage(INVITATION, target, channelName);
					Utils::sendPrivateMessage(invitation, client, target);
				}
			}
			else
				Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, channelName);
			
		}
		else
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[2]);

	}
	else
	{
		client->setLastArg(cmd[1]);
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
	}
}
