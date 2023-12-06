/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:45:58 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 11:52:38 by mat              ###   ########.fr       */
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
	
	std::cout << "hola" << std::endl;
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
	if (!cmd[1].empty())
	{
		channelSubArgs = getSubArgs(cmd[1]);
		//if (!cmd[2].empty())
		//	keySubArgs = getSubArgs(cmd[2]);
		for (std::vector<std::string>::iterator it1 = channelSubArgs.begin(); it1 != channelSubArgs.end(); it1++)
		{
			const std::map<std::string, Channel *>::iterator it2 = _channels.find(CHANNEL_PREFIX + *it1);
			if (it2 == _channels.end())
			{
				printLog("adding : " + *it1);
				Channel *channel = new Channel(*it1, client);
				_channels[CHANNEL_PREFIX + *it1] = channel; 
				sendJoinMessage(channel, client, *it1);
			} else {
				printLog("joining : " + *it1);
				it2->second->addNewUser(client);
				sendJoinMessage(it2->second, client, *it1);
			}
		}
	}
}
