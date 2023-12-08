/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/08 16:05:24 by rbroque          ###   ########.fr       */
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
	char					 restChar;

	while (iss >> token && token[0] != SETTER_CHAR) {
		tokens.push_back(token);
	}
	if (iss.eof() == false) {  // found a separator
		while (iss.get(
			restChar))	// concatenate the separator with the end of the line
			token += restChar;
		tokens.push_back(token);
	} else if (token[0] ==
			   SETTER_CHAR)	 // the final token is beginning with a separator
		tokens.push_back(token);
	return tokens;
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
	_cmdMap["JOIN"] = &Server::join;
	_cmdMap["PRIVMSG"] = &Server::privmsg;
	_cmdMap["PART"] = &Server::part;
	_cmdMap["MODE"] = &Server::mode;
	_cmdMap["TOPIC"] = &Server::topic;

	printLog("Port: " + port);
	printLog("Password: " + password);
	_socket.setup();
	_epollFd = epoll_create1(0);
	addFdToPoll(_socket.getSocketFd());
}

Server::~Server() {
	delFdToPoll(_socket.getSocketFd());
	if (_epollFd != 0)
		close(_epollFd);
	for (std::map<std::string, Channel *>::iterator it = _channels.begin();
		 it != _channels.end(); ++it) {
		delete it->second;
	}
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
	for (std::map<std::string, Channel *>::iterator it = _channels.begin();
		 it != _channels.end(); ++it)
		it->second->removeUser(client);
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
			printLog(CLOSED_CLIENT_MESSAGE);
		}
	} catch (ReadFailException &e) {
		printLog(e.what());
	}
}

/////////////
// PRIVATE //
/////////////

void Server::printLog(const std::string &logMessage) const {
	std::cout << GREY << logMessage << NC << std::endl;
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

//	Command handling methods

void Server::processReceivedData(
	const std::string &received_data, const int clientFd) {
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

void Server::handleClientMessage(
	const std::string &message, Client *const client) {
	const std::vector<std::string> cmd = getCommandTokens(message);

	if (cmd.empty() || cmd[0].empty())
		return;
	client->setLastCmd(cmd[0]);
	if (cmd.size() == 1 || cmd[1].empty())
		client->setLastArg("");
	else
		client->setLastArg(cmd[1]);
	if (client->isAuthenticated() == false)
		getUserLogin(cmd, client);
	else
		handleCmd(cmd, client);
}

void Server::handleCmd(
	const std::vector<std::string> &cmd, Client *const client) {
	const std::map<std::string, CommandFunction>::iterator it =
		_cmdMap.find(cmd[0]);
	const CommandFunction fct = it->second;

	try {
		if (it != _cmdMap.end())
			(this->*fct)(cmd, client);
		else
			Utils::sendFormattedMessage(ERR_UNKNOWNCOMMAND, client);
	} catch (std::string &e) {	// Catch only command exception
		std::cout << client << ": " << e << std::endl;
		Utils::sendFormattedMessage(e, client);
	}
}

void Server::tryPasswordAuth(
	const std::vector<std::string> &cmd, Client *const client) {
	static const std::string passCommand = "PASS";

	if (cmd[0] == passCommand) {
		pass(cmd, client);
	}
}

void Server::setClientLogAssets(
	const std::vector<std::string> &cmd, Client *const client) {
	static const std::string userCommand = "USER";
	static const std::string nickCommand = "NICK";

	if (cmd[0] == userCommand) {
		user(cmd, client);
	} else if (cmd[0] == nickCommand) {
		nick(cmd, client);
	}
}

void Server::getUserLogin(
	const std::vector<std::string> &cmd, Client *const client) {
	const unsigned int logMask = client->getLogMask();

	if (logMask == EMPTY_LOGIN) {
		cap(cmd, client);
	} else if (logMask == CAP_LOGIN) {
		tryPasswordAuth(cmd, client);
	} else if (logMask & (CAP_LOGIN | PASS_LOGIN)) {
		setClientLogAssets(cmd, client);
	}
	if (client->isAuthenticated()) {
		Utils::sendFormattedMessage(RPL_WELCOME, client);
		printLog("Client is authenticated: Nickname[" + client->getNickname() +
				 "]; Username[" + client->getUsername() + "]");
	}
}

// Exceptions

const char *Server::ListenFailException::what() const throw() {
	return (LISTEN_FAIL__ERROR);
}

const char *Server::ReadFailException::what() const throw() {
	return (READ_FAIL__ERROR);
}

const char *Server::InvalidLoginCommandException::what() const throw() {
	return (WRONG_CMD__ERROR);
}
