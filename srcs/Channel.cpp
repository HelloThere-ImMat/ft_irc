/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/11 09:16:49 by rbroque          ###   ########.fr       */
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
	: _name(name), _mode(INVITE_ONLY | TOPIC_RESTRICTION) {
	const SpecifiedClient spClient = {.client = client, .isOp = true};

	userMap[client->getNickname()] = spClient;
}

Channel::~Channel() { userMap.clear(); }

void Channel::addNewUser(const Client *const client,
	const std::vector<std::string> &keys, const size_t keyIndex) {
	const size_t keysSize = keys.size();
	if (this->isFull())
		throw TooManyUserException();
	if (_mode.isKeyProtected() &&
		(keyIndex >= keysSize || keys[keyIndex] != _password))
		throw WrongChannelKeyException();
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

const std::string &Channel::getName() const { return _name; }

void Channel::setTopic(const std::string &newTopic) { _topic = newTopic; }

bool Channel::processMode(
	std::vector<std::string> &cmd, const Client *const client) {
	const std::string modeString = cmd[2];
	t_modSetter		  setter = ADD;
	modeStatus		  status = {.hasChanged = false, .doesUseArg = false};
	size_t			  argsIndex = 3;
	bool			  hasChanged = false;

	for (std::string::const_iterator it = modeString.begin();
		 it != modeString.end(); ++it) {
		if (*it == '+' || *it == '-') {
			setter = (*it == '+') ? ADD : RM;
		} else {
			status = _mode.setMode(setter, *it, cmd, argsIndex);
			if (status.doesUseArg) {
				if (status.hasChanged) {
					status.hasChanged =
						canModeBeApplied(*it, cmd[argsIndex], setter, client);
				}
				++argsIndex;
			}
			hasChanged |= status.hasChanged;
		}
	}
	return status.hasChanged;
}

bool Channel::isAbleToJoin(const std::vector<std::string> &cmd) const {
	return (_mode.isKeyProtected() == false) ||
		   (cmd.size() > 2 && cmd[2] == _password);
}

void Channel::sendToOthers(
	const Client *const client, const std::string message) const {
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		if (it->second.client->getSocketFd() != client->getSocketFd())
			Utils::sendPrivateMessage(message, client, it->second.client);
	}
}

void Channel::sendToAll(
	const Client *const client, const std::string message) const {
	for (std::map<std::string, SpecifiedClient>::const_iterator it =
			 userMap.begin();
		 it != userMap.end(); it++) {
		Utils::sendPrivateMessage(message, client, it->second.client);
	}
}

void Channel::sendTopic(const Client *const client) const {
	if (_topic.empty())
		Utils::sendFormattedMessage(RPL_NOTOPIC, client, _name);
	else {
		const std::string formatRPL =
			Utils::getFormattedMessage(RPL_TOPIC, client, _name) + _topic;
		Utils::sendMessage(formatRPL, client);
	}
}

void Channel::sendTopicToAll(const Client *const client) const {
	const std::string formatRPL =
		Utils::getFormattedMessage(RPL_TOPIC, client, _name);
	sendToAll(client, formatRPL + _topic);
}

void Channel::sendMode(const Client *const client) const {
	const std::string rplMessage =
		Utils::getFormattedMessage(RPL_CHANNELMODEIS, client, _name);
	const std::string modeMessage = rplMessage + _mode.getModeMessage();
	Utils::sendMessage(modeMessage, client);
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
	return (_mode.isTopicProtected() == false || isOp(client));
}

bool Channel::isOp(const Client *const client) const {
	if (isUserInChannel(client) == false)
		return false;
	const std::string nickname = client->getNickname();

	return (userMap.find(nickname)->second.isOp);
}

/////////////////////
// Private Methods //
/////////////////////

bool Channel::canModeBeApplied(const char c, std::string &arg,
	const t_modSetter setter, const Client *const client) {
	if (c == KEY_CHAR)
		_password = arg;
	else if (c == USRLIMIT_CHAR) {
		_userlimit = atoi(arg.c_str());
		std::stringstream ss;
		ss << _userlimit;
		arg = ss.str();
	} else if (c == OP_CHANGE_CHAR) {
		std::map<std::string, SpecifiedClient>::iterator it = userMap.find(arg);
		if (it == userMap.end()) {
			Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client);
			return false;
		} else
			it->second.isOp = (setter == ADD);
	}
	return true;
}

bool Channel::isFull() const {
	return _mode.hasUserLimit() && userMap.size() >= _userlimit;
}