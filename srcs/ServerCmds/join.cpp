/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:45:58 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 20:18:06 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Static

static int countOccurence(char c, std::string &strRef)
{
	int		OccurenceNb = 0;
	size_t	size = strRef.size();
	size_t	i = 0;

	while (i < size)
	{
		if (strRef[i] == c)
			OccurenceNb++;
		i++;
	}
	return (OccurenceNb);
}

std::vector<std::string> getSubArgs(std::string list)
{
	std::vector<std::string> subArg;
	int subArgNb = countOccurence(',', list) + 1;
	int i = 0;
	int startPos = 0;
	
	while (i < subArgNb)
	{
		int sepPos = list.find(',', startPos);
		subArg.push_back(list.substr(startPos, sepPos - startPos));
		startPos = sepPos + 1;
		i++;
	}
	return (subArg);
}

// Join Methods

void Server::sendJoinMessage(const Channel *const channel, const Client *client, const std::string &channelName)
{
	std::string channelUserList;
	
	channel->sendToAll(client, JOIN_PREFIX + channelName);
	//if (!channel->topic.empty())
	//	SendCmd::sendFormattedMessage(TOPIC_JOIN_MESSAGE + channel->topic, client);
	channelUserList = channel->getUserList();
	SendCmd::sendFormattedMessage(UL_JOIN_MESSAGE + channelUserList, client, channelName);
	SendCmd::sendFormattedMessage(EUL_JOIN_MESSAGE, client, channelName);
}

void Server::join(const std::vector<std::string> &cmd, Client *const client) {
	
	std::vector<std::string> channelSubArgs;
	std::vector<std::string> keySubArgs;
	size_t					 sizeCmd = cmd.size();
	if (sizeCmd < 2) {
		SendCmd::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return;
	}
	else {
		channelSubArgs = getSubArgs(cmd[1]);
		size_t channelNb = channelSubArgs.size();
		for (size_t i = 0; i < channelNb; i++)
		{
			std::string channelName = channelSubArgs[i];
			const std::map<std::string, Channel *>::iterator it2 = _channels.find(channelName);
			if (it2 == _channels.end())
			{
				if ((channelName)[0] != CHANNEL_PREFIX)
					SendCmd::sendFormattedMessage(ERR_BADCHANMASK, client, channelName);
				else if (_channels.size() >= MAX_CHANNEL_NB)
					SendCmd::sendFormattedMessage(ERR_TOOMANYCHANNELS, client, channelName);
				else {
					Channel *channel = new Channel(channelName, client);
					_channels[channelName] = channel; 
					sendJoinMessage(channel, client, channelName);
				}
			} else {
				if (sizeCmd > 2)
					keySubArgs = getSubArgs(cmd[2]);
				if (it2->second->addNewUser(client, keySubArgs, i) == EXIT_SUCCESS)
					sendJoinMessage(it2->second, client, channelName);
				else
					SendCmd::sendFormattedMessage(ERR_BADCHANNELKEY, client, channelName);	
			}
		}
	}
}
