/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 00:21:11 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Static

static std::string getSpecifiedNick(const SpecifiedClient &spClient) {
	if (spClient.isOp)
		return (OP_PREFIX + spClient.client->getNickname());
	return (spClient.client->getNickname());
}

static std::vector<std::string> getModArgs(
	const std::vector<std::string> &cmd) {
	const size_t			 size = cmd.size();
	std::vector<std::string> modArgs;

	for (size_t i = 3; i < size; ++i) {
		modArgs.push_back(cmd[i]);
	}
	return modArgs;
}

// Methods

Channel::Channel(const std::string &name, const Client *const client)
	: _name(name),
	  _isTopicProtected(true),
	  _modMask(INVITE_ONLY | TOPIC_RESTRICTION) {
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

void Channel::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_modMask |= flags;
	else if (setter == RM)
		_modMask &= ~flags;
}

bool Channel::setMode(
	const t_modSetter setter, const char c, std::vector<std::string> &modArgs) {
	// if (c == 'k' || c == 'o' || c == 'l')
	// {
	// 	if (c == 'l')

	// 	modArgs.erase(0, 1);
	(void)modArgs;
	if (c == 'i' || c == 't') {
		const uint8_t flags = (c == 'i') ? INVITE_ONLY : TOPIC_RESTRICTION;
		setFlags(flags, setter);
		return true;
	}
	return false;
}

bool Channel::processMode(
	const std::vector<std::string> &cmd, const Client *const client) {
	const std::string		 modString = cmd[2];
	std::vector<std::string> modArgs = getModArgs(cmd);
	t_modSetter				 setter = ADD;
	bool					 hasChanged = false;

	for (std::string::const_iterator it = modString.begin();
		 it != modString.end(); ++it) {
		if (*it == '+')
			setter = ADD;
		else if (*it == '-')
			setter = RM;
		hasChanged |= setMode(setter, *it, modArgs);
	}
	(void)client;
	return hasChanged;
}

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

void Channel::sendMode(const Client *const client) const {
	const std::string rplMessage =
		SendCmd::getFormattedMessage(RPL_CHANNELMODEIS, client);
	const std::string modeMessage = rplMessage + getModeMessage();
	SendCmd::sendMessage(modeMessage, client);
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

std::string Channel::getModeMessage() const {
	std::string modeMessage = "+";

	if (_modMask & INVITE_ONLY)
		modeMessage += "i";
	if (_modMask & TOPIC_RESTRICTION)
		modeMessage += "t";
	if (_modMask & PASS_ONLY)
		modeMessage += "k";
	if (_modMask & USERLIMIT)
		modeMessage += "l";
	return modeMessage;
}

bool Channel::isOp(const Client *const client) const {
	if (isUserInChannel(client) == false)
		return false;
	const std::string nickname = client->getNickname();

	return (userMap.find(nickname)->second.isOp);
}
