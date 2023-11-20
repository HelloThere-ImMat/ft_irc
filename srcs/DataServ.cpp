/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataServ.cpp                                           :+:      :+:    :+:   */
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

DataServ::DataServ(const std::string &port) {

	if (isPortValid(port) == false)
		throw InvalidPortException();
	else
		initAddress(std::atoi(port.c_str()));
}

DataServ::DataServ(const int port) {
	initAddress(port);
}

void	DataServ::setup() {

	const int	option = DEFAULT_OPTION;

	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SockInitException();
	if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
		throw SockOptException();
	if (bind(servfd, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw SockBindException();
}

void	DataServ::acceptConnection() {

	addrlen = sizeof(address);
	sockfd = accept(servfd,
						(struct sockaddr*)&address, &addrlen);
	if (sockfd < 0)
		throw CantAcceptException();
}

int	DataServ::getSocketFd() const {
	return (sockfd);
}

int	DataServ::getServerSocketFd() const {
	return (servfd);
}

void	DataServ::setSocketFd(const int newFd) {
	sockfd = newFd;
}

DataServ::~DataServ() {
	close(sockfd);
	close(servfd);
}

/////////////
// PRIVATE //
/////////////

void	DataServ::initAddress(const int port) {

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
}

////////////////
// Exceptions //
////////////////

const char	*DataServ::SockInitException::what() const throw() {
	return (SOCKET_INIT__ERROR);
}

const char	*DataServ::SockOptException::what() const throw() {
	return (SOCKET_OPT__ERROR);
}

const char	*DataServ::SockBindException::what() const throw() {
	return (SOCKET_BIND__ERROR);
}

const char	*DataServ::InvalidPortException::what() const throw() {
	return (INVALID_PORT__ERROR);
}

const char	*DataServ::CantAcceptException::what() const throw() {
	return (CANT_ACCEPT__ERROR);
}
