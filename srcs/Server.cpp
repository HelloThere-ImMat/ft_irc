/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/28 10:37:10 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

////////////
// STATIC //
////////////

static std::vector<std::string> getCommandTokens(
	const std::string &ircMessage) {
	std::vector<std::string> tokens;
	std::istringstream		 iss(ircMessage);
	std::string				 token;

	while (iss >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

////////////
// PUBLIC //
////////////

Server::Server(const std::string &port, const std::string &password)
	: _socket(port), _password(password) {
	std::cout << "Port is " << port << std::endl;
	std::cout << "Password is " << password << std::endl;
}

Server::~Server() {
	delFdToPoll(_socket.getSocketFd());
	for (std::map<int, Client *>::iterator it = _clientMap.begin();
		 it != _clientMap.end(); ++it) {
		delete it->second;
	}
}

void Server::start() {
	_socket.setup();
	_epollFd = epoll_create1(0);
	addFdToPoll(_socket.getSocketFd());
}

void Server::listen() const {
	const int servfd = _socket.getSocketFd();

	if (::listen(servfd, MAX_CLIENT_COUNT) < 0)
		throw ListenFailException();
}

void Server::addNewClient() {
	const int newFd = _socket.acceptNewConnectionSocket();

	addFdToPoll(newFd);
	_clientMap[newFd] = new Client(newFd);
	sendMessage(WELCOME_MESSAGE, newFd);
}

void Server::lookForEvents() {
	struct epoll_event events[MAX_CLIENT_COUNT];
	const int		   servfd = _socket.getSocketFd();
	const int		   eventCount =
		epoll_wait(_epollFd, events, MAX_CLIENT_COUNT, TIMEOUT);

	for (int i = 0; i < eventCount; ++i) {
		int eventFd = events[i].data.fd;

		if (eventFd == servfd)
			addNewClient();
		else
			readClientCommand(eventFd);
	}
}

void Server::readClientCommand(const int sockfd) {
	const std::string clientBuffer = _clientMap[sockfd]->getBuffer();
	static char		  buffer[BUFFER_SIZE] = {0};
	ssize_t			  bytes_received;

	memset(buffer, 0, BUFFER_SIZE - 1);
	if ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL)) >
		0) {
		std::string received_data(buffer, bytes_received);

		if (!clientBuffer.empty()) {
			received_data = clientBuffer + received_data;
			_clientMap[sockfd]->clearBuffer();
		}
		processReceivedData(received_data, sockfd);
	} else if (bytes_received < 0) {
		throw ReadFailException();
	} else {
		std::cout << "Client left" << std::endl;
		delFdToPoll(sockfd);
	}
}

void Server::sendMessage(const std::string &message, const int clientFd) const {
	const std::string formatMessage = message + END_MESSAGE;

	if (send(clientFd, formatMessage.c_str(), formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << "Sent message: " << message << std::endl;
}

/////////////
// PRIVATE //
/////////////

void Server::sendError(const std::string &message, const int clientFd) const {
	const std::string formatErrorMessage = ERROR_PREFIX + message;
	sendMessage(formatErrorMessage, clientFd);
}

void Server::addFdToPoll(const int fd) {
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = fd;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, event.data.fd, &event);
}

void Server::delFdToPoll(const int fd) {
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = fd;
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, event.data.fd, &event);
}

void Server::processReceivedData(const std::string &received_data,
								 const int			clientFd) {
	Client *const client = _clientMap[clientFd];
	size_t		  start_pos = 0;
	size_t		  end_pos = received_data.find(END_MESSAGE, start_pos);

	while (end_pos != std::string::npos) {
		std::string ircMessage =
			received_data.substr(start_pos, end_pos - start_pos);
		std::cout << "Received IRC message: " << ircMessage << std::endl;

		// Process the IRC message (e.g., parse and handle different IRC
		// commands)

		if (client->isAuthenticated() == false)
			getUserLogin(ircMessage, client);
		// ... (Implement IRC message handling logic here)

		start_pos = end_pos + 2;  // Move to the start of the next IRC message
		end_pos = received_data.find(END_MESSAGE, start_pos);
	}

	// Check for any remaining incomplete message and buffer it
	if (start_pos < received_data.length()) {
		std::string incompleteMessage = received_data.substr(start_pos);
		_clientMap[clientFd]->setBuffer(incompleteMessage);
	}
}

void Server::getUserLogin(const std::string &ircMessage, Client *const client) {
	static const std::string loginCmdTab[] = {"CAP", "PASS", "NICK", "USER"};
	const std::vector<std::string> tokens = getCommandTokens(ircMessage);
	size_t						   i;

	for (i = 0; i < 4; i++) {
		if (tokens[0] == loginCmdTab[i])
			break;
	}
	try {
		switch (i) {
			case 0:
				client->addToLoginMask(CAP_LOGIN);
				std::cout << "CAP added to mask" << std::endl;
				break;
			case 1:
				if (tokens[1] == _password) {
					std::cout << "Valid Password!" << std::endl;
					client->addToLoginMask(PASS_LOGIN);
				} else {
					sendError(WRONG_PASS__ERROR, client->getSocketFd());
				}
				break;
			case 2:
				client->setNickname(tokens[1]);
				client->addToLoginMask(NICK_LOGIN);
				std::cout << "NICK added to mask" << std::endl;
				break;
			case 3:
				client->setUsername(tokens[1]);
				client->addToLoginMask(USER_LOGIN);
				std::cout << "USER added to mask" << std::endl;
				break;
		}
		if (client->isAuthenticated())
			std::cout << "Client is authenticated: NickName["
					  << client->getNickname() << "]; Username["
					  << client->getUserName() << "]" << std::endl;

	} catch (std::exception &e) {  // change by Client::Exception
		std::cout << e.what() << std::endl;
		// change by sendError(e.what(), client->getSocketFd());
	}
}

// Exceptions

const char *Server::ListenFailException::what() const throw() {
	return (LISTEN_FAIL__ERROR);
}

const char *Server::ReadFailException::what() const throw() {
	return (READ_FAIL__ERROR);
}

const char *Server::SendFailException::what() const throw() {
	return (SEND_FAIL__ERROR);
}
