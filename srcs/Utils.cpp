/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:55:59 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 22:07:29 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// Static Funcs

static std::string replacePatterns(const std::string &original,
	const std::string &pattern, const std::string &replacement) {
	size_t		startPos = 0;
	std::string newStr = original;

	while ((startPos = newStr.find(pattern, startPos)) != std::string::npos) {
		newStr.replace(startPos, pattern.length(), replacement);
		startPos += replacement.length();
	}
	return newStr;
}

// String Methods

std::vector<std::string> Utils::splitString(
	const std::string &listStr, const char delimiter) {
	std::vector<std::string> strings;
	std::string				 string;
	std::istringstream		 tokenStream(listStr);

	while (std::getline(tokenStream, string, delimiter))
		strings.push_back(string);
	return (strings);
}

std::string Utils::getFullMessage(
	const std::vector<std::string> &cmd, const size_t startIndex) {
	std::string	 fullMessage;
	const size_t size = cmd.size();
	size_t		 i = startIndex;
	if (size > startIndex) {
		while (i < size) {
			if (i > startIndex)
				fullMessage += MESSAGE_SEPARATOR;
			fullMessage += cmd[i++];
		}
	}
	return (fullMessage);
}

void Utils::removeDuplicateChars(std::string &str) {
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

bool Utils::isSetter(const char c) { return c == '+' || c == '-'; }

// Send Methods

void Utils::sendMessage(
	const std::string &message, const Client *const client) {
	static const std::string domainName = DOMAIN_NAME;
	const int				 clientFd = client->getSocketFd();
	const std::string		 formatMessage =
		":" + domainName + " " + message + END_MESSAGE;

	if (send(clientFd, formatMessage.c_str(), formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << GREEN << OUTMES_PREFIX << NC << message << std::endl;
}

void Utils::sendPrivateMessage(const std::string &message,
	const Client *const sender, const Client *const receiver) {
	const std::string senderSpec =
		sender->getNickname() + "!~" + sender->getUsername() + "@localhost";
	const std::string formatMessage =
		":" + senderSpec + " " + message + END_MESSAGE;

	if (send(receiver->getSocketFd(), formatMessage.c_str(),
			formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << RED << OUTMES_PREFIX << NC << formatMessage << std::endl;
}

void Utils::sendFormattedMessage(const std::string &message,
	const Client *const client, std::string channelName) {
	sendMessage(getFormattedMessage(message, client, channelName), client);
}

// Format methods

std::string Utils::getFormattedMessage(const std::string &message,
	const Client *const client, std::string channelName) {
	const std::string mapPattern[PATTERN_COUNT][2] = {
		{"<networkname>", NETWORK_NAME}, {"<servername>", SERVER_NAME},
		{"<client>", client->getNickname()}, {"<nick>", client->getNickname()},
		{"<command>", client->getLastCmd()}, {"<arg>", client->getLastArg()},
		{"<username>", client->getUsername()}, {"<hostname>", HOST_NAME},
		{"<channelName>", channelName}};

	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT; ++i) {
		formattedMessage = replacePatterns(
			formattedMessage, mapPattern[i][0], mapPattern[i][1]);
	}
	return formattedMessage;
}

bool Utils::isThereInvalidChar(
	const std::string &str, const std::string &invalidCharset) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (invalidCharset.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

// Exceptions

const char *Utils::SendFailException::what() const throw() {
	return (SEND_FAIL_ERROR);
}
