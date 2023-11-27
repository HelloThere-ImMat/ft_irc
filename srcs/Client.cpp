/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:30:35 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/27 16:35:24 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _sockfd(0) {}

Client::Client(const int sockfd) : _sockfd(sockfd) {}

Client::~Client() {}

int Client::getSocketFd() const { return (_sockfd); }

const std::string &Client::getBuffer() const { return (_buffer); }

void Client::setBuffer(const std::string &incompleteMessage) {
	_buffer = incompleteMessage;
}

void Client::clearBuffer() { _buffer.clear(); }
