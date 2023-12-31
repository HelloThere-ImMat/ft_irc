/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 17:36:59 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/13 17:33:43 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Static

static std::vector<const Client *>::iterator getClientFromList(
	std::vector<const Client *> &lst, const Client *const client) {
	for (std::vector<const Client *>::iterator it = lst.begin();
		 it != lst.end(); it++) {
		if (*it == client)
			return it;
	}
	return lst.end();
}

static std::vector<const Client *>::const_iterator getClientFromList(
	const std::vector<const Client *> &lst, const Client *const client) {
	for (std::vector<const Client *>::const_iterator it = lst.begin();
		 it != lst.end(); it++) {
		if (*it == client)
			return it;
	}
	return lst.end();
}

static std::string getSpecifiedNick(const SpecifiedClient &spClient) {
	if (spClient.isOp)
		return (OP_PREFIX + spClient.client->getNickname());
	return (spClient.client->getNickname());
}

static void cleanModeMessage(std::string &mode) {
	Utils::removeDuplicateChars(mode);
	const size_t len = mode.length();
	if (len > 0 && len <= 2 && Utils::isSetter(mode[0]) &&
		(len == 1 || Utils::isSetter(mode[1]))) {
		mode = "";
	}
}

static char getSetterChar(const t_modSetter setter) {
	return setter == ADD ? '+' : '-';
}

static std::vector<std::string> initNewModeVect(
	const std::vector<std::string> &cmd) {
	std::vector<std::string> newModeVect;

	newModeVect.push_back(cmd[0]);
	newModeVect.push_back(cmd[1]);
	newModeVect.push_back("");
	return newModeVect;
}

// Methods

Channel::Channel(const std::string &name, const Client *const client)
	: _name(name), _mode(NO_MOD) {
	const SpecifiedClient spClient = {.client = client, .isOp = true};

	userMap[client->getNickname()] = spClient;
}

Channel::~Channel() { userMap.clear(); }

void Channel::addNewUser(const Client *const client,
	const std::vector<std::string> &keys, const size_t keyIndex) {
	if (this->isFull())
		throw TooManyUserException();
	if (isPassValid(keys, keyIndex) == false)
		throw WrongChannelKeyException();
	if (isWelcome(client) == false)
		throw NotInvitedException();
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

std::vector<std::string> Channel::processMode(
	const std::vector<std::string> &cmd, Client *const client) {
	const std::string		 modeString = cmd[2];
	std::vector<std::string> newModeVect = initNewModeVect(cmd);
	modeStatus	status = {.hasChanged = false, .doesUseArg = false};
	t_modSetter setter = ADD;
	size_t		argsIndex = START_MODE_INDEX;

	for (std::string::const_iterator it = modeString.begin();
		 it != modeString.end(); ++it) {
		if (Utils::isSetter(*it)) {
			setter = (*it == '+') ? ADD : RM;
		} else {
			status = _mode.setMode(setter, *it, cmd, argsIndex);
			if (status.hasChanged && status.doesUseArg)
				status.hasChanged =
					tryModeApplication(setter, *it, cmd[argsIndex], client);
			if (status.hasChanged) {
				newModeVect[2] += getSetterChar(setter);
				newModeVect[2] += *it;
				if (status.doesUseArg)
					newModeVect.push_back(cmd[argsIndex]);
			}
			argsIndex += status.doesUseArg;
		}
	}
	cleanModeMessage(newModeVect[2]);
	return newModeVect;
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

bool Channel::canInvite(const Client *const client) const {
	return (_mode.isInviteOnly() == false || isOp(client));
}

bool Channel::isOp(const Client *const client) const {
	if (isUserInChannel(client) == false)
		return false;
	const std::string nickname = client->getNickname();

	return (userMap.find(nickname)->second.isOp);
}

bool Channel::isInInviteList(const Client *const client) const {
	return getClientFromList(_inviteList, client) != _inviteList.end();
}

void Channel::addToInviteList(const Client *const client) {
	_inviteList.push_back(client);
}

/////////////////////
// Private Methods //
/////////////////////

bool Channel::tryModeApplication(const t_modSetter setter, const char cflag,
	std::string arg, Client *const client) {
	bool hasChanged = false;

	try {
		setModeParameter(cflag, arg, setter, client);
		hasChanged = true;
	} catch (UserNotInChannelException &e) {
		Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client, _name);
	}
	return hasChanged;
}

void Channel::setModeParameter(const char c, std::string &arg,
	const t_modSetter setter, Client *const client) {
	if (c == KEY_CHAR) {
		_mode.setPassword(arg);
	} else if (c == USRLIMIT_CHAR) {
		const int userlimit = atoi(arg.c_str());
		_mode.setUserlimit(userlimit);
		std::stringstream ss;
		ss << userlimit;
		arg = ss.str();
	} else if (c == OP_CHANGE_CHAR) {
		const std::map<std::string, SpecifiedClient>::iterator it =
			userMap.find(arg);
		if (it == userMap.end()) {
			client->setLastArg(arg);
			throw UserNotInChannelException();
		} else
			it->second.isOp = (setter == ADD);
	}
}

bool Channel::isPassValid(
	const std::vector<std::string> &cmd, const size_t passIndex) const {
	return (_mode.isKeyProtected() == false) ||
		   (cmd.size() > passIndex && cmd[passIndex] == _mode.getPassword());
}

bool Channel::isWelcome(const Client *const client) {
	if (_mode.isInviteOnly() == false)
		return (true);
	const std::vector<const Client *>::iterator it =
		getClientFromList(_inviteList, client);
	if (it == _inviteList.end())
		return (false);
	_inviteList.erase(it);
	return (true);
}

bool Channel::isFull() const {
	return _mode.hasUserLimit() && userMap.size() >= _mode.getUserLimit();
}
