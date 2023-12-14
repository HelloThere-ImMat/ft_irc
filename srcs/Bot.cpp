/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:36:54 by mdorr             #+#    #+#             */
/*   Updated: 2023/12/14 15:36:24 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

static bool hasAllreadyInteracted(const Client *const client, std::vector<const Client *> &botUsers)
{
	for (std::vector<const Client *>::iterator it = botUsers.begin(); it != botUsers.end(); it++)
	{
		if (*it == client)
			return (true);
	}
	return (false);
}

//void Bot::respond(const Client *const client)
//{

//}

void Bot::removeUserFromList(const Client *const client)
{
	std::vector<const Client *>::iterator it = _botUsers.begin();

	while (it != _botUsers.end())
	{
		if (*it == client)
			break;
		++it;
	}
	if (it != _botUsers.end())
		_botUsers.erase(it);
}

void Bot::sendMessage(const Client *const client, const char *message)
{
	std::string formattedMess = PRIVMSG_PREFIX + client->getNickname() + " " + message;
	Utils::sendPrivateMessage(formattedMess, *this, client);
}

void Bot::interact(const Client *const client)
{

	if (hasAllreadyInteracted(client, _botUsers)) {
		std::cout << "hellooo" << std::endl;
	}
	else {
		_botUsers.push_back(client);
		std::string message = Utils::getFormattedMessage(BOT_INTRO_MESS, client);
		sendMessage(client, message.c_str());
		sendMessage(client, BOT_INTRO_MESS2);
		sendMessage(client, BOT_INTRO_MESS3);
		sendMessage(client, BOT_INTRO_MESS4);
	}
}


const std::string &Bot::getName() const {
	return (_name);
}

Bot::Bot() : _name(BOT_NAME)
{
	_loadingResponses.push_back("Loading...");
	_loadingResponses.push_back("..");
	_loadingResponses.push_back("...");
	_loadingResponses.push_back("hhmmmmmm..");
	_loadingResponses.push_back("Let me think...");
}
