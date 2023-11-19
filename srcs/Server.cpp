/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 19:03:56 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// PUBLIC //
////////////

Server::Server(const char *const port): _socket(std::atoi(port)) {

	bzero(this->buffer, BUFFER_SIZE);
}

Server::~Server() {}

int	Server::start() {
	return (_socket.setup());
}

int	Server::listen() {

	const int	servfd = _socket.getServerSocketFd();

	if (::listen(servfd, MAX_CLIENT_COUNT) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Server::acceptConnection() {

	return (_socket.acceptConnection());
}

int	Server::readMessage() {

	const int	sockfd = _socket.getSocketFd();

	if (read(sockfd, buffer, BUFFER_SIZE - 1) < 0)	// subtract 1 for the null
		return (EXIT_FAILURE);
	std::cout << "Message received : " << buffer << std::endl;
	return (EXIT_SUCCESS);
}

int	Server::sendMessage(const std::string &message) {

	const int	sockfd = _socket.getSocketFd();

	if (send(sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0) < 0)
		return (EXIT_FAILURE);
	std::cout << "Message sent: " << message << std::endl;
	return (EXIT_SUCCESS);
}
