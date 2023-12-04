/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:30:35 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/04 15:16:20 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>
#include <string.h>

// Constructors

Client::Client(const int sockfd) : _sockfd(sockfd), _loginMask(EMPTY_LOGIN) {}

Client::Client(const Client &ref) : _sockfd(ref.getSocketFd()), _loginMask(ref.getLogMask())
{
	_nickname = ref.getNickname();
	_username = ref.getUsername();
	_buffer = ref.getBuffer();
}

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

// Setters

void Client::setNickname(const std::string &nickname) { _nickname = nickname; };

void Client::setUsername(const std::string &username) { _username = username; };

void Client::setLastCmd(const std::string &lastCmd) { _lastCmd = lastCmd; };

void Client::setLastArg(const std::string &lastArg) { _lastArg = lastArg; };

void Client::setBuffer(const std::string &incompleteMessage) {
	_buffer = incompleteMessage;
}

void Client::addToLoginMask(const uint8_t mask) { _loginMask |= mask; }

// Members

bool Client::isAuthenticated() const { return _loginMask == LOGGED; }

void Client::clearBuffer() { _buffer.clear(); }
