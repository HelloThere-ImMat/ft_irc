/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:09:20 by mat               #+#    #+#             */
/*   Updated: 2023/12/09 02:53:19 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kick(const std::vector<std::string> &cmd, Client *const client)
{
	(void)cmd;
	(void)client;
	const size_t cmdSize = cmd.size();
	if (cmdSize < 3)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	};
	std::map<std::string, Channel *>::iterator itMap = _channels.find(cmd[1]);
	if (itMap!= _channels.end())
	{
		Channel *channel = itMap->second;
		const std::vector<std::string> users = Utils::splitString(cmd[2], CMD_ARG_SEPARATOR);
		for (std::vector<std::string>::const_iterator itUser = users.begin(); itUser != users.end(); itUser++) {
			Client *kickedUser =_clientMap.getClient(*itUser); 
			if (kickedUser != NULL) {
				if (channel->isUserInChannel(client)) {
					if (channel->isOp(client)){
						if (channel->isUserInChannel(kickedUser)) {
							client->setLastArg(*itUser);
							const std::string kickMessage = Utils::getFormattedMessage(KICK, client, cmd[1]);
							Utils::sendPrivateMessage(kickMessage, client, kickedUser);
						} else {
							client->setLastArg(*itUser);
							Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client , cmd[1]); 
						}
					} else
						Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, cmd[1]);
				} else
					Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, cmd[1]);
			}
			else
			{
				client->setLastArg((*itUser));
				Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
			}
		}
	}
	else
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
}
