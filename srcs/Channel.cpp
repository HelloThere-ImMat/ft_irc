/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 22:01:33 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Static

static std::string getSpecifiedNick(const SpecifiedClient &spClient) {
	if (spClient.isOp)
		return (OP_PREFIX + spClient.client->getNickname());
	return (spClient.client->getNickname());
}

// Methods

Channel::Channel(const std::string &name, const Client *const client)
	: _name(name), _isTopicProtected(true), _isPasswordProtected(false) {
	const SpecifiedClient spClient = {.client = client, .isOp = true};

	userMap[client->getNickname()] = spClient;
}

Channel::~Channel() { userMap.clear(); }

int Channel::addNewUser(const Client *const client, std::vector<std::string> &keys, size_t keyIndex) {
	SpecifiedClient spClient = {.client = client, .isOp = false};

	if (_isPasswordProtected && keys.empty())
		return (EXIT_FAILURE);
	else if (_isPasswordProtected && keys[keyIndex] != _password)
		return (EXIT_FAILURE);
	
	userMap[client->getNickname()] = spClient;
	return (EXIT_SUCESS);
}

const std::string Channel::getUserList() const {
	std::string userList;
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		if (it != userMap.begin())
			userList += USERLIST_SEPARATOR;
		userList += getSpecifiedNick(it->second);
	}
	return (userList);
}

void Channel::removeUser(const Client *const client) {
	std::string nickname = client->getNickname();
	userMap.erase(nickname);
}

const std::string &Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string &newTopic) { _topic = newTopic; }

void Channel::sendToOthers(
	const Client *const client, const std::string message) const {
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		if (it->second.client->getSocketFd() != client->getSocketFd())
			SendCmd::sendPrivateMessage(message, client, it->second.client);
	}
}

void Channel::sendToAll(
	const Client *const client, const std::string message) const {
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		SendCmd::sendPrivateMessage(message, client, it->second.client);
	}
}

void Channel::sendTopic(const Client *const client) const {
	if (_topic.empty())
		SendCmd::sendFormattedMessage(RPL_NOTOPIC, client);
	else {
		const std::string formatRPL =
			SendCmd::getFormattedMessage(RPL_TOPIC, client) + _topic;
		SendCmd::sendMessage(formatRPL, client);
	}
}

void Channel::sendTopicToAll(const Client *const client) const {
	const std::string formatRPL =
		SendCmd::getFormattedMessage(RPL_TOPIC, client);
	sendToAll(client, formatRPL + _topic);
}

bool Channel::isUserInChannel(const Client *const client) const {
	std::string nick = client->getNickname();
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		if (it->second.client->getNickname() == nick)
			return (true);
	}
	return (false);
}

bool Channel::canChangeTopic(const Client *const client) const {
	return (_isTopicProtected == false || isOp(client));
}

/////////////
// PRIVATE //
/////////////

bool Channel::isOp(const Client *const client) const {
	if (isUserInChannel(client) == false)
		return false;
	const std::string nickname = client->getNickname();

	return (userMap.find(nickname)->second.isOp);
}
