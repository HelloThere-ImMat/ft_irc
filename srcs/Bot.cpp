/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:36:54 by mdorr             #+#    #+#             */
/*   Updated: 2023/12/14 17:47:12 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot() : _name(BOT_NAME) {}

static std::string loadingResponses[] = {
	"Loading...", "..", "...", "hhmmmmmm..", "Let me think..."};

// Static functions

static size_t getRandNb(const size_t range) {
	size_t rNb;

	std::srand(time(NULL));
	rNb = rand() % range;
	return (rNb);
}

static bool hasAlreadyInteracted(
	const Client *const client, const std::vector<const Client *> &botUsers) {
	for (std::vector<const Client *>::const_iterator it = botUsers.begin();
		 it != botUsers.end(); it++) {
		if (*it == client)
			return (true);
	}
	return (false);
}

// Public methods

void Bot::removeUserFromList(const Client *const client) {
	std::vector<const Client *>::iterator it = _botUsers.begin();

	while (it != _botUsers.end() && *it != client) ++it;
	if (it != _botUsers.end())
		_botUsers.erase(it);
}

void Bot::interact(const Client *const client, const std::string &message) {
	if (hasAlreadyInteracted(client, _botUsers)) {
		respond(client, message);
	} else {
		_botUsers.push_back(client);
		sendIntroMessage(client);
	}
}

const std::string &Bot::getName() const { return (_name); }

// Private methods
void Bot::sendMessage(
	const Client *const client, const char *const message) const {
	const std::string formattedMess =
		PRIVMSG_PREFIX + client->getNickname() + " " + message;
	Utils::sendPrivateMessage(formattedMess, *this, client);
}

void Bot::respond(
	const Client *const client, const std::string &message) const {
	if (message.find('?') != std::string::npos)
		sendMessage(client, BOT_Q_ANS);
	else {
		const size_t loadingPrintsNb = getRandNb(4);
		for (size_t i = 0; i < loadingPrintsNb; i++) {
			sleep(BOT_TIME_LOAD);
			const size_t loadingIndex = getRandNb(LOADING_RESP_NB);
			sendMessage(client, loadingResponses[loadingIndex].c_str());
		}
		sleep(BOT_TIME_SUSPENSE);
		sendMessage(client, BOT_BASIC_ANS);
	}
}

void Bot::sendIntroMessage(const Client *const client) const {
	const std::string formattedIntroMessage =
		Utils::getFormattedMessage(BOT_INTRO_MESS, client);
	sendMessage(client, formattedIntroMessage.c_str());
	sendMessage(client, BOT_INTRO_MESS2);
	sendMessage(client, BOT_INTRO_MESS3);
	sendMessage(client, BOT_INTRO_MESS4);
}