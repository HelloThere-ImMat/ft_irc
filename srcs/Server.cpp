/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 19:11:41 by rbroque          ###   ########.fr       */
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
	_epollFd = epoll_create1(0);
	addFdToPoll(_socket.getSocketFd());
}

void	Server::listen() const {

	const int	servfd = _socket.getSocketFd();

	if (::listen(servfd, MAX_CLIENT_COUNT) < 0)
		throw ListenFailException();
}

void	Server::addNewClient() {

	const int	newFd = _socket.acceptNewConnectionSocket();

	addFdToPoll(newFd);
	_client.setSocketFd(newFd);
	sendMessage(WELCOME_MESSAGE);
}

void	Server::lookForEvents() {

	struct epoll_event	events[MAX_CLIENT_COUNT];
	const int			eventCount = epoll_wait(_epollFd, events, MAX_CLIENT_COUNT, TIMEOUT);
	const int			servfd = _socket.getSocketFd();

	for (int i = 0; i < eventCount; ++i) {
		int	eventFd = events[i].data.fd;

		if (eventFd == servfd)
			addNewClient();
		else
			readClientCommand(eventFd);
	}
}

void	Server::readClientCommand(const int sockfd) {

	static char		buffer[BUFFER_SIZE] = {0};
	ssize_t			bytes_received;

	bzero(buffer, BUFFER_SIZE - 1);
	if ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL)) > 0)
	{
		std::string	received_data(buffer, bytes_received);
		std::string clientBuffer = _client.getBuffer();

		if (!clientBuffer.empty()) {
			received_data = clientBuffer + received_data;
			_client.setBuffer("");
		}
		processReceivedData(received_data);
	}
	else if (bytes_received < 0)	// subtract 1 for the null
		throw ReadFailException();
	else if (bytes_received == 0)
	{
		std::cout << "Client left" << std::endl;
		delFdToPoll(sockfd);
	}
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

void	Server::addFdToPoll(const int fd)
{
	struct epoll_event	event;

	event.events = EPOLLIN;
	event.data.fd = fd;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, event.data.fd, &event);
}

void	Server::delFdToPoll(const int fd)
{
	struct epoll_event	event;

	event.events = EPOLLIN;
	event.data.fd = fd;
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, event.data.fd, &event);
}

void	Server::processReceivedData(const std::string &received_data) {
	size_t start_pos = 0;
	size_t end_pos = received_data.find(END_MESSAGE, start_pos);

	while (end_pos != std::string::npos) {
		std::string irc_message = received_data.substr(start_pos, end_pos - start_pos);
		std::cout << "Received IRC message: " << irc_message << std::endl;

		// Process the IRC message (e.g., parse and handle different IRC commands)
		// ... (Implement IRC message handling logic here)

		start_pos = end_pos + 2; // Move to the start of the next IRC message
		end_pos = received_data.find(END_MESSAGE, start_pos);
	}

	// Check for any remaining incomplete message and buffer it
	if (start_pos < received_data.length()) {
		std::string incompleteMessage = received_data.substr(start_pos);
		_client.setBuffer(incompleteMessage);
	}
}

// Exceptions

const char* Server::ListenFailException::what() const throw() {
	return (LISTEN_FAIL__ERROR);
}

const char* Server::ReadFailException::what() const throw() {
	return (READ_FAIL__ERROR);
}

const char* Server::SendFailException::what() const throw() {
	return (SEND_FAIL__ERROR);
}

