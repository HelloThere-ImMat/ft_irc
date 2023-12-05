/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:55:59 by mat               #+#    #+#             */
/*   Updated: 2023/12/05 17:19:24 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SendCmd.hpp"

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

static std::string getFormattedMessage(
	const std::string &message, const Client *const client) {
	const std::string mapPattern[PATTERN_COUNT][2] = {
		{"<networkname>", NETWORK_NAME}, {"<servername>", SERVER_NAME},
		{"<client>", client->getNickname()}, {"<nick>", client->getNickname()},
		{"<command>", client->getLastCmd()}, {"<arg>", client->getLastArg()},
		{"<username>", client->getUsername()}, {"<hostname>", HOST_NAME}};

	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT; ++i) {
		formattedMessage = replacePatterns(
			formattedMessage, mapPattern[i][0], mapPattern[i][1]);
	}
	return formattedMessage;
}

// Send Methods

void SendCmd::sendMessage(const std::string &message, const int clientFd) {
	static const std::string domainName = DOMAIN_NAME;
	const std::string		 formatMessage =
		":" + domainName + " " + message + END_MESSAGE;

	if (send(clientFd, formatMessage.c_str(), formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << GREEN << OUTMES_PREFIX << NC << message << std::endl;
}

void SendCmd::sendPrivateMessage(const std::string &message,
	const Client *const sender, const Client *const receiver) {
	const std::string senderSpec =
		sender->getNickname() + "!~" + sender->getUsername() + "@localhost";
	const std::string formatMessage = getFormattedMessage(
		":" + senderSpec + " " + message + END_MESSAGE, sender);

	if (send(receiver->getSocketFd(), formatMessage.c_str(),
			formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << RED << OUTMES_PREFIX << NC << formatMessage << std::endl;
}

void SendCmd::sendFormattedMessage(
	const std::string &message, const Client *const client) {
	sendMessage(getFormattedMessage(message, client), client->getSocketFd());
}

const char *SendCmd::SendFailException::what() const throw() {
	return (SEND_FAIL__ERROR);
}
