/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 18:25:20 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// PUBLIC //
////////////

Server::Server(const char *const port): _socket(std::atoi(port)) {

	bzero(this->buffer, BUFFER_SIZE);
	this->option = 1;
}

Server::~Server() {}

int	Server::setup() {

	try {
		if ((_socket.servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw SockInitException();
		if (setsockopt(_socket.servfd, SOL_SOCKET, SO_REUSEADDR, &(this->option), sizeof(option)) < 0)
			throw SockOptException();
		if (bind(_socket.servfd, (struct sockaddr*)&_socket.address, sizeof(_socket.address)) < 0)
			throw SockBindException();
	}
	catch(std::exception &e)
	{
		std::cerr << "Error : Setup : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Server::listen() {

	if (::listen(_socket.servfd, MAX_CLIENT_COUNT) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Server::acceptConnection() {

	_socket.sockfd = accept(_socket.servfd,
						(struct sockaddr*)&_socket.address, &_socket.addrlen);
	if (_socket.sockfd < 0)
	{
		std::cerr << "Error : while accepting" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Server::readMessage() {

	if (read(_socket.sockfd, buffer, 1024 - 1) < 0)	// subtract 1 for the null
		return (EXIT_FAILURE);
	std::cout << "Message received : " << buffer << std::endl;
	return (EXIT_SUCCESS);
}

int	Server::sendMessage(const std::string &message) {

	if (send(_socket.sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0) < 0)
		return (EXIT_FAILURE);
	std::cout << "Message sent: " << message << std::endl;
	return (EXIT_SUCCESS);
}

/////////////
// PRIVATE //
/////////////

const char	*Server::SockInitException::what() const throw(){
	return (SOCKET_INIT__ERROR);
}

const char	*Server::SockOptException::what() const throw(){
	return (SOCKET_OPT__ERROR);
}

const char	*Server::SockBindException::what() const throw(){
	return (SOCKET_BIND__ERROR);
}
