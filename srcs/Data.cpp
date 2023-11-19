/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:21 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 21:39:39 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"

//////////////////////
// Static functions //
//////////////////////

static bool	isPortValid(const std::string &port) {
	for (std::string::const_iterator it = port.begin(); it != port.end(); ++it) {
		if (!isdigit(*it)) {
			return false;
		}
	}
	return true;
}

////////////
// PUBLIC //
////////////

Data::Data(const std::string &port) {

	if (isPortValid(port) == false)
		throw InvalidPortException();
	else
		initAddress(std::atoi(port.c_str()));
}

Data::Data(const int port) {
	initAddress(port);
}

void	Data::setup() {

	const int	option = DEFAULT_OPTION;

	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SockInitException();
	if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
		throw SockOptException();
	if (bind(servfd, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw SockBindException();
}

void	Data::acceptConnection() {

	sockfd = accept(servfd,
						(struct sockaddr*)&address, &addrlen);
	if (sockfd < 0)
		throw CantAcceptException();
}

int	Data::getSocketFd() const {
	return (sockfd);
}

int	Data::getServerSocketFd() const {
	return (servfd);
}

void	Data::setSocketFd(const int newFd) {
	sockfd = newFd;
}

Data::~Data() {
	close(sockfd);
	close(servfd);
}

/////////////
// PRIVATE //
/////////////

void	Data::initAddress(const int port) {

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
}

////////////////
// Exceptions //
////////////////

const char	*Data::SockInitException::what() const throw() {
	return (SOCKET_INIT__ERROR);
}

const char	*Data::SockOptException::what() const throw() {
	return (SOCKET_OPT__ERROR);
}

const char	*Data::SockBindException::what() const throw() {
	return (SOCKET_BIND__ERROR);
}

const char	*Data::InvalidPortException::what() const throw() {
	return (INVALID_PORT__ERROR);
}

const char	*Data::CantAcceptException::what() const throw() {
	return (CANT_ACCEPT__ERROR);
}
