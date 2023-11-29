/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/29 09:23:29 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <sstream>
#include <vector>

#include "Client.hpp"
#include "DataServ.hpp"
#include "irc.hpp"

#define BUFFER_SIZE		   1024
#define TIMEOUT			   -1
#define MAX_CLIENT_COUNT   3
#define ERROR_PREFIX	   "Error: "
#define WELCOME_MESSAGE	   "Hello from the Server\n"
#define LISTEN_FAIL__ERROR "listening failed"
#define READ_FAIL__ERROR   "reading failed"
#define SEND_FAIL__ERROR   "sending failed"
#define END_MESSAGE		   "\r\n"
#define WRONG_PASS__ERROR  "Invalid password!"
#define WRONG_CMD__ERROR   "Invalid Login Command!"

class Server {

	typedef void (Server::*CommandFunction)(const std::vector<std::string>&, Client *const);

   public:
	Server(const std::string &port, const std::string &password);
	~Server();
	void start();
	void listen() const;
	void lookForEvents();
	void sendMessage(const std::string &message, const int clientFd) const;
	void readClientCommand(const int fd);
	void addNewClient();

   private:
	// Attributes
	DataServ				_socket;
	int						_epollFd;
	std::string				_password;
	std::map<int, Client *> _clientMap;
	std::map<std::string, CommandFunction> _cmdMap;
	// Private Methods
	void sendError(const std::string &message, const int clientFd) const;
	void addFdToPoll(const int fd);
	void delFdToPoll(const int fd);
	void sendRPL(Client *const client) const;
	void processReceivedData(const std::string &received_data,
							 const int			clientFd);
	void getUserLogin(const std::string &ircMessage, Client *const client);
	void handleCmd(const std::string &ircsMessage, Client *const client);
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
	class InvalidLoginCommandException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};

