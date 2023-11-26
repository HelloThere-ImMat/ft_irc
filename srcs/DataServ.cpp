/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataServ.cpp                                           :+:      :+:    :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:21 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 16:56:08 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DataServ.hpp"

//////////////////////
// Static functions //
//////////////////////

static bool isPortValid(const std::string &port) {
	for (std::string::const_iterator it = port.begin(); it != port.end();
		 ++it) {
		if (!isdigit(*it)) {
			return false;
		}
	}
	return true;
}

////////////
// PUBLIC //
////////////

DataServ::DataServ(const std::string &port) {
	if (isPortValid(port) == false)
		throw InvalidPortException();
	else
		initAddress(std::atoi(port.c_str()));
}

DataServ::DataServ(const int port) { initAddress(port); }

void DataServ::setup() {
	const int option = DEFAULT_OPTION;

	if ((_servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SockInitException();
	if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) <
		0)
		throw SockOptException();
	if (bind(_servfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw SockBindException();
}

int DataServ::acceptNewConnectionSocket() {
	int sockfd;

	_addrlen = sizeof(_address);
	sockfd = accept(_servfd, (struct sockaddr *)&_address, &_addrlen);
	if (sockfd < 0)
		throw CantAcceptException();
	return (sockfd);
}

int DataServ::getSocketFd() const { return (_servfd); }

DataServ::~DataServ() { close(_servfd); }

/////////////
// PRIVATE //
/////////////

void DataServ::initAddress(const int port) {
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
}

////////////////
// Exceptions //
////////////////

const char *DataServ::SockInitException::what() const throw() {
	return (SOCKET_INIT__ERROR);
}

const char *DataServ::SockOptException::what() const throw() {
	return (SOCKET_OPT__ERROR);
}

const char *DataServ::SockBindException::what() const throw() {
	return (SOCKET_BIND__ERROR);
}

const char *DataServ::InvalidPortException::what() const throw() {
	return (INVALID_PORT__ERROR);
}

const char *DataServ::CantAcceptException::what() const throw() {
	return (CANT_ACCEPT__ERROR);
}
