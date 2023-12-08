/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/08 00:53:29 by rbroque          ###   ########.fr       */
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
	: _name(name),
	  _isTopicProtected(true),
	  _mode(INVITE_ONLY | TOPIC_RESTRICTION) {
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

bool Channel::processMode(std::vector<std::string> &cmd) {
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
			hasChanged |= status.hasChanged;
			if (status.doesUseArg) {
				applyMode(*it, cmd[argsIndex]);
				++argsIndex;
			}
		}
	}
	return status.hasChanged;
}

bool Channel::isAbleToJoin(const std::vector<std::string> &cmd) const {
	return !(_mode.getModeMask() & PASS_ONLY) ||
		   (cmd.size() > 2 && cmd[2] == _password);
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
	const std::string modeMessage = rplMessage + _mode.getModeMessage();
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

bool Channel::isOp(const Client *const client) const {
	if (isUserInChannel(client) == false)
		return false;
	const std::string nickname = client->getNickname();

	return (userMap.find(nickname)->second.isOp);
}

/////////////////////
// Private Methods //
/////////////////////

void Channel::applyMode(const char c, std::string &arg) {
	if (c == KEY_CHAR)
		_password = arg;
	else if (c == USRLIMIT_CHAR) {
		_userlimit = atoi(arg.c_str());
		std::stringstream ss;
		ss << _userlimit;
		arg = ss.str();
	}
}