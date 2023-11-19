/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 21:24:00 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// PUBLIC //
////////////

Server::Server(
	const std::string &port,
	const std::string &password): _socket(port), _password(password) {

	bzero(this->buffer, BUFFER_SIZE);
	std::cout << "Port is " << port << std::endl;
	std::cout << "Password is " << password << std::endl;
}

Server::~Server() {}

void	Server::start() {
	_socket.setup();
}

void	Server::listen() {

	const int	servfd = _socket.getServerSocketFd();

	if (::listen(servfd, MAX_CLIENT_COUNT) < 0)
		throw ListenFailException();
}

void	Server::acceptConnection() {

	_socket.acceptConnection();
}

void	Server::readMessage() {

	const int	sockfd = _socket.getSocketFd();

	if (read(sockfd, buffer, BUFFER_SIZE - 1) < 0)	// subtract 1 for the null
		throw ReadFailException();
	else
		std::cout << "Message received : " << buffer << std::endl;
}

void	Server::sendMessage(const std::string &message) {

	const int	sockfd = _socket.getSocketFd();

	if (send(sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0) < 0)
		throw SendFailException();
	else
		std::cout << "Message sent: " << message << std::endl;
}

/////////////
// PRIVATE //
/////////////

const char* Server::ListenFailException::what() const throw() {
	return (LISTEN_FAIL__ERROR);
}

const char* Server::ReadFailException::what() const throw() {
	return (READ_FAIL__ERROR);
}

const char* Server::SendFailException::what() const throw() {
	return (SEND_FAIL__ERROR);
}
