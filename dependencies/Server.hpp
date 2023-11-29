/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/29 17:08:48 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <sstream>
#include <vector>

#include "Client.hpp"
#include "DataServ.hpp"
#include "irc.hpp"

// Count

#define BUFFER_SIZE		 1024
#define TIMEOUT			 -1
#define PATTERN_COUNT	 5
#define MAX_CLIENT_COUNT 3

// STRINGS

#define DOMAIN_NAME	 "ft_irc.local"
#define NETWORK_NAME "Server IRC"
#define SERVER_NAME	 "IRC"
#define END_MESSAGE	 "\r\n"

// RPL

#define RPL_WELCOME "001 <client> :Welcome to the <networkname> Network, <nick>"

// Message

#define WELCOME_MESSAGE "Hello from the Server\n"

// Errors

#define ERROR_PREFIX	   "Error: "
#define LISTEN_FAIL__ERROR "listening failed"
#define READ_FAIL__ERROR   "reading failed"
#define SEND_FAIL__ERROR   "sending failed"
#define WRONG_PASS__ERROR  "Invalid password!"
#define MISS_PASS__ERROR   "Password is missing!"
#define WRONG_CMD__ERROR   "Invalid Login Command!"

class Server {
	typedef void (Server::*CommandFunction)(const std::vector<std::string> &,
											Client *const);

   public:
	Server(const std::string &port, const std::string &password);
	~Server();
	void start();
	void listen() const;
	void lookForEvents();
	void readClientCommand(const int fd);
	void addNewClient();

   private:
	// Attributes
	DataServ							   _socket;
	int									   _epollFd;
	std::map<std::string, CommandFunction> _cmdMap;
	std::string							   _password;
	std::map<int, Client *>				   _clientMap;
	// Private Methods
	void processReceivedData(const std::string &received_data,
							 const int			clientFd);
	//    Send Methods
	void sendMessage(const std::string &message, const int clientFd) const;
	void sendFormattedMessage(const std::string	&message,
							  const Client *const client) const;
	void sendError(const std::string &message, const int clientFd) const;
	//    Poll Methods
	void addFdToPoll(const int fd);
	void delFdToPoll(const int fd);
	//    Cmd Methods
	void handleClientMessage(const std::string &message, Client *const client);
	void handleCmd(const std::vector<std::string> &cmd, Client *const client);
	void getUserLogin(const std::vector<std::string> &cmd,
					  Client *const					  client);
	//    Log Methods
	void tryPasswordAuth(const std::vector<std::string> &cmd,
						 Client *const					 client);
	void setClientLogAssets(const std::vector<std::string> &cmd,
							Client *const					client);
	// CMD
	void cap(const std::vector<std::string> &cmd, Client *const client);
	void pass(const std::vector<std::string> &cmd, Client *const client);
	void user(const std::vector<std::string> &cmd, Client *const client);
	void nick(const std::vector<std::string> &cmd, Client *const client);
	void ping(const std::vector<std::string> &cmd, Client *const client);
	// Exceptions
	class ListenFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class ReadFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class SendFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class InvalidPasswordException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class MissingPasswordException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class InvalidLoginCommandException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};
