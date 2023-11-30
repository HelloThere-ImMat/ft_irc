/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/30 21:59:18 by rbroque          ###   ########.fr       */
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

static std::string replacePatterns(std::string		  original,
								   const std::string &pattern,
								   const std::string &replacement) {
	size_t startPos = 0;
	while ((startPos = original.find(pattern, startPos)) != std::string::npos) {
		original.replace(startPos, pattern.length(), replacement);
		startPos += replacement.length();
	}
	return original;
}

static std::string getFormattedMessage(const std::string	 &message,
									   const Client *const client) {
	const std::string mapPattern[PATTERN_COUNT][2] = {
		{"<networkname>", NETWORK_NAME},	 {"<servername>", SERVER_NAME},
		{"<client>", client->getUsername()}, {"<nick>", client->getNickname()},
		{"<command>", client->getLastCmd()}, {"<arg>", client->getLastArg()}};
	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT; ++i) {
		formattedMessage = replacePatterns(formattedMessage, mapPattern[i][0],
										   mapPattern[i][1]);
	}
	return formattedMessage;
}

////////////
// PUBLIC //
////////////

Server::Server(const std::string &port, const std::string &password)
	: _socket(port), _password(password) {
	_cmdMap["PASS"] = &Server::pass;
	_cmdMap["USER"] = &Server::user;
	_cmdMap["NICK"] = &Server::nick;
	_cmdMap["CAP"] = &Server::cap;
	_cmdMap["PING"] = &Server::ping;

	std::cout << "Port is " << port << std::endl;
	std::cout << "Password is " << password << std::endl;
}

Server::~Server() {
	delFdToPoll(_socket.getSocketFd());
	close(_epollFd);
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
	_clientMap.addClient(new Client(newFd));
}

void Server::closeClient(Client *const client) {
	const int clientFd = client->getSocketFd();

	delFdToPoll(clientFd);
	_clientMap.eraseClient(client);
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
	Client *const	  client = _clientMap.getClient(sockfd);
	const std::string clientBuffer = client->getBuffer();
	static char		  buffer[BUFFER_SIZE] = {0};

	memset(buffer, 0, BUFFER_SIZE - 1);
	try {
		ssize_t bytes_received;
		if ((bytes_received =
				 recv(sockfd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL)) > 0) {
			std::string received_data(buffer, bytes_received);

			if (!clientBuffer.empty()) {
				received_data = clientBuffer + received_data;
				client->clearBuffer();
			}
			processReceivedData(received_data, sockfd);
		} else if (bytes_received < 0) {
			throw ReadFailException();
		} else {
			closeClient(client);
			throw ClosedClientException();
		}
	} catch (ClosedClientException &e) {
		std::cout << e.what() << std::endl;
	} catch (ReadFailException &e) {
		std::cout << e.what() << std::endl;
	}
}

/////////////
// PRIVATE //
/////////////

//	Send Methods

void Server::sendMessage(const std::string &message, const int clientFd) const {
	static const std::string domainName = DOMAIN_NAME;
	const std::string		 formatMessage =
		":" + domainName + " " + message + END_MESSAGE;

	if (send(clientFd, formatMessage.c_str(), formatMessage.size(), 0) < 0)
		throw SendFailException();
	else
		std::cout << GREEN << OUTMES_PREFIX << NC << message << std::endl;
}

void Server::sendFormattedMessage(const std::string	&message,
								  const Client *const client) const {
	sendMessage(getFormattedMessage(message, client), client->getSocketFd());
}

//	Poll Methods

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

//	Commands methods

void Server::handleClientMessage(const std::string &message,
								 Client *const		client) {
	const std::vector<std::string> cmd = getCommandTokens(message);

	if (cmd.empty() || cmd[0].empty())
		return;
	client->setLastCmd(cmd[0]);
	if (cmd[1].empty())
		client->setLastArg("");
	else
		client->setLastArg(cmd[1]);
	if (client->isAuthenticated() == false)
		getUserLogin(cmd, client);
	else
		handleCmd(cmd, client);
}

void Server::processReceivedData(const std::string &received_data,
								 const int			clientFd) {
	Client *const client = _clientMap.getClient(clientFd);
	size_t		  start_pos = 0;
	size_t		  end_pos = received_data.find(END_MESSAGE, start_pos);

	while (end_pos != std::string::npos) {
		std::string ircMessage =
			received_data.substr(start_pos, end_pos - start_pos);
		std::cout << BLUE << INMES_PREFIX << NC << ircMessage << std::endl;
		handleClientMessage(ircMessage, client);
		start_pos = end_pos + 2;
		end_pos = received_data.find(END_MESSAGE, start_pos);
	}
	if (start_pos < received_data.length()) {
		std::string incompleteMessage = received_data.substr(start_pos);
		client->setBuffer(incompleteMessage);
	}
}

void Server::handleCmd(const std::vector<std::string> &cmd,
					   Client *const				   client) {
	const std::map<std::string, CommandFunction>::iterator it =
		_cmdMap.find(cmd[0]);
	const CommandFunction fct = it->second;

	try {
		if (it != _cmdMap.end())
			(this->*fct)(cmd, client);
	} catch (std::string &e) {	// Catch only command exception
		std::cout << client << ": " << e << std::endl;
		sendFormattedMessage(e, client);
	}
}

void Server::tryPasswordAuth(const std::vector<std::string> &cmd,
							 Client *const					 client) {
	static const std::string passCommand = "PASS";

	if (cmd[0] == passCommand) {
		pass(cmd, client);
	}
}

void Server::setClientLogAssets(const std::vector<std::string> &cmd,
								Client *const					client) {
	static const std::string userCommand = "USER";
	static const std::string nickCommand = "NICK";

	if (cmd[0] == userCommand) {
		user(cmd, client);
	} else if (cmd[0] == nickCommand) {
		nick(cmd, client);
	}
}

void Server::getUserLogin(const std::vector<std::string> &cmd,
						  Client *const					  client) {
	const unsigned int logMask = client->getLogMask();

	if (logMask == EMPTY_LOGIN) {
		cap(cmd, client);
	} else if (logMask == CAP_LOGIN) {
		tryPasswordAuth(cmd, client);
	} else if (logMask & (CAP_LOGIN | PASS_LOGIN)) {
		setClientLogAssets(cmd, client);
	}
	if (client->isAuthenticated()) {
		sendFormattedMessage(RPL_WELCOME, client);
		std::cout << "Client is authenticated: Nickname["
				  << client->getNickname() << "]; Username["
				  << client->getUsername() << "]" << std::endl;
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

const char *Server::InvalidLoginCommandException::what() const throw() {
	return (WRONG_CMD__ERROR);
}

const char *Server::ClosedClientException::what() const throw() {
	return (CLOSED_CLIENT__ERROR);
}
