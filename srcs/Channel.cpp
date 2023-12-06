/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 10:39:56 by rbroque          ###   ########.fr       */
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
	: _name(name), _isTopicProtected(false) {
	const SpecifiedClient spClient = {.client = client, .isOp = true};

	userMap[client->getNickname()] = spClient;
}

Channel::~Channel() { userMap.clear(); }

void Channel::addNewUser(const Client *const client) {
	SpecifiedClient spClient = {.client = client, .isOp = false};

	userMap[client->getNickname()] = spClient;
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
	return (_isTopicProtected == true || isOp(client));
}

void Channel::sendTopic(const Client *const client) const {
	const std::string topic = getTopic();
	if (topic.empty())
		SendCmd::sendFormattedMessage(RPL_NOTOPIC, client);
	else
		SendCmd::sendFormattedMessage(RPL_TOPIC + topic, client);
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
