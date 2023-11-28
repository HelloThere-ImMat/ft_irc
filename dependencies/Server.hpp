/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/28 22:55:09 by rbroque          ###   ########.fr       */
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
#define MAX_CLIENT_COUNT 3

// STRINGS

#define SERVER_NAME "IRC"
#define END_MESSAGE "\r\n"

// RPL

#define RPL_WELCOME "001"

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
	DataServ				_socket;
	int						_epollFd;
	std::string				_name;
	std::string				_password;
	std::map<int, Client *> _clientMap;
	// Private Methods
	//    Send Methods
	void sendMessage(const std::string &message, const int clientFd) const;
	void sendWelcomeMessage(const Client *const client) const;
	void sendError(const std::string &message, const int clientFd) const;
	//    Poll Methods
	void addFdToPoll(const int fd);
	void delFdToPoll(const int fd);
	//    Log Methods
	void processReceivedData(const std::string &received_data,
							 const int			clientFd);
	void getUserLogin(const std::string &irc_message, Client *const client);
	void startClientAuth(const std::vector<std::string> &cmd,
						 Client *const					 client);
	void tryPasswordAuth(const std::vector<std::string> &cmd,
						 Client *const					 client);
	void setClientLogAssets(const std::vector<std::string> &cmd,
							Client *const					client);
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
