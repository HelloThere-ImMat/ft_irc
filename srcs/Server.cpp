/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 18:23:36 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// PUBLIC //
////////////

Server::Server(
	const std::string &port,
	const std::string &password): _socket(port), _password(password) {

	std::cout << "Port is " << port << std::endl;
	std::cout << "Password is " << password << std::endl;
}

Server::~Server() {}

void	Server::start() {
	_socket.setup();
}

void	Server::listen() const {

	const int	servfd = _socket.getSocketFd();

	if (::listen(servfd, MAX_CLIENT_COUNT) < 0)
		throw ListenFailException();
}

void	Server::acceptConnection() {

	_client.setSocketFd(_socket.getNewConnectionSocket());
}

void	Server::readMessage() {

	static char	buffer[BUFFER_SIZE] = {0};
	const int	sockfd = _client.getSocketFd();
	ssize_t		byte_read_count;

	bzero(buffer, BUFFER_SIZE - 1);
	byte_read_count = recv(sockfd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL);
	if (byte_read_count < 0)	// subtract 1 for the null
		throw ReadFailException();
	else
		std::cout << "Message received : " << buffer << std::endl;
}

void	Server::sendMessage(const std::string &message) const {

	const int	sockfd = _client.getSocketFd();

	if (send(sockfd, message.c_str(), message.size(), 0) < 0)
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
