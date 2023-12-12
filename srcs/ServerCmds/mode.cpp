/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 00:10:53 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/12 00:27:34 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Static //

static void removeDuplicateChars(std::string &str) {
	bool charSet[256] = {false};  // Assuming ASCII characters

	size_t currentIndex = 0;
	size_t len = str.length();

	for (size_t i = 0; i < len; ++i) {
		char currentChar = str[i];
		if (!charSet[static_cast<unsigned char>(currentChar)]) {
			charSet[static_cast<unsigned char>(currentChar)] = true;
			str[currentIndex++] = currentChar;
		}
	}
	str.resize(currentIndex);
}

static void keepOnlySpecificChars(
	std::string &str, const std::string &allowedChars) {
	for (std::string::iterator it = str.begin(); it != str.end();) {
		if (allowedChars.find(*it) == std::string::npos) {
			it = str.erase(it);
		} else {
			++it;
		}
	}
}

static void setModeStr(std::string &str) {
	keepOnlySpecificChars(str, MODE_SETCHAR);
	removeDuplicateChars(str);
	if (str.empty() == false) {
		char last = str[str.length() - 1];
		if (last == '-' || last == '+')
			str.erase(str.length() - 1);
	}
}

static bool isTargetChannel(const std::string &target) {
	return target[0] == CHANNEL_PREFIX;
}

static void applyModeOnChannel(Channel *const channel,
	const std::vector<std::string> &cmd, Client *const client) {
	std::vector<std::string> cmdCopy = cmd;

	if (cmdCopy.size() == MODE_ASK_SIZE) {
		channel->sendMode(client);
	} else if (channel->isOp(client) == false) {
		Utils::sendFormattedMessage(
			ERR_CHANOPRIVSNEEDED, client, channel->getName());
	} else {
		setModeStr(cmdCopy[2]);
		if (channel->processMode(cmdCopy, client))
			channel->sendToAll(client, Utils::getFullMessage(cmdCopy, 0));
	}
}

// Public methods

void Server::mode(const std::vector<std::string> &cmd, Client *const client) {
	const size_t size = cmd.size();

	if (size < 2) {
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
	} else if (isTargetChannel(cmd[1])) {
		const std::map<std::string, Channel *>::iterator it =
			_channels.find(cmd[1]);
		if (it == _channels.end()) {
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, cmd[1]);
		} else {
			applyModeOnChannel(it->second, cmd, client);
		}
	}
}