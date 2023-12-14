/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:30:35 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/14 16:19:03 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Constructors

Client::Client(const int sockfd) : _sockfd(sockfd), _loginMask(EMPTY_LOGIN) {}

// Destructor

Client::~Client() { close(_sockfd); }

// Getters

int Client::getSocketFd() const { return (_sockfd); }

const std::string &Client::getNickname() const { return (_nickname); }

const std::string &Client::getUsername() const { return (_username); }

const std::string &Client::getLastCmd() const { return (_lastCmd); }

const std::string &Client::getLastArg() const { return (_lastArg); }

const std::string &Client::getBuffer() const { return (_buffer); }

uint8_t Client::getLogMask() const { return (_loginMask); }

bool Client::isInChannel(const Channel *const channel) const {
	const std::string channelName = channel->getName();

	return _chanMap.find(channelName) != _chanMap.end();
}

// Setters

void Client::setNickname(const std::string &nickname) { _nickname = nickname; };

void Client::setUsername(const std::string &username) { _username = username; };

void Client::setLastCmd(const std::string &lastCmd) { _lastCmd = lastCmd; };

void Client::setLastArg(const std::string &lastArg) { _lastArg = lastArg; };

void Client::setBuffer(const std::string &incompleteMessage) {
	_buffer = incompleteMessage;
}

void Client::addToLoginMask(const uint8_t mask) { _loginMask |= mask; }

void Client::addToChanMap(const Channel *const newChannel) {
	const std::string newChannelName = newChannel->getName();
	_chanMap[newChannelName] = newChannel;
}

void Client::rmFromChanMap(const Channel *const newChannel) {
	const std::string newChannelName = newChannel->getName();
	_chanMap.erase(newChannelName);
}

// Members

bool Client::isAuthenticated() const { return _loginMask == LOGGED; }

void Client::sendToChannels(const std::string &message) const {
	for (std::map<std::string, const Channel *>::const_iterator it =
			 _chanMap.begin();
		 it != _chanMap.end(); ++it) {
		it->second->sendToOthers(this, message);
	}
}

void Client::clearBuffer() { _buffer.clear(); }
