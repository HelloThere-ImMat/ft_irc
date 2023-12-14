/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:30:08 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/14 14:58:34 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// STATIC //
////////////

static bool isAuthorizedChar(const char c) {
	static const std::string specialAuthorizedSet = SPECIAL_NICK_CHARSET;

	return isalnum(c) || specialAuthorizedSet.find(c) != std::string::npos;
}

static bool areOnlyAuthorizedChar(const std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (isAuthorizedChar(str[i]) == false)
			return false;
	}
	return true;
}

static bool isNicknameValid(const std::string &nickname) {
	return isdigit(nickname[0]) == false && areOnlyAuthorizedChar(nickname);
}

/////////////
// PRIVATE //
/////////////

bool Server::isNicknameAlreadyUsed(const std::string &nickname) {
	return _clientMap.getClient(nickname) != NULL;
}

void Server::notifyInConv(const Client *const client, const std::string message) const {

	std::vector<Conversation>::const_iterator it = _convList.begin();

	while (it != _convList.end()) {
		if (it->user1 == client)
			Utils::sendPrivateMessage(message, it->user1, it->user2);
		else if (it->user2 == client)
			Utils::sendPrivateMessage(message, it->user2, it->user1);
		++it;
	}
}

void Server::notifyNickUpdate(const Client *const client, const std::string newNick) const {
	const std::string nickUpdateMessage = NICK_PREFIX + newNick;
	Utils::sendPrivateMessage(nickUpdateMessage, client, client);
	client->sendToChannels(nickUpdateMessage);
	notifyInConv(client, nickUpdateMessage);
}

void Server::nick(const std::vector<std::string> &cmd, Client *const client) {
	if (cmd[1].empty())
		Utils::sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
	else if (isNicknameValid(cmd[1]) == false)
		Utils::sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
	else if (isNicknameAlreadyUsed(cmd[1]))
		Utils::sendFormattedMessage(ERR_NICKNAMEINUSE, client);
	else {
		if (client->isAuthenticated())
			notifyNickUpdate(client, cmd[1]);
		client->setNickname(cmd[1]);
		_clientMap.updateClientNickname(client, cmd[1]);
		client->addToLoginMask(NICK_LOGIN);
	}
}
