/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/30 23:09:08 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <sstream>
#include <vector>

#include "ClientManager.hpp"
#include "DataServ.hpp"
#include "irc.hpp"

// Count

#define BUFFER_SIZE		 1024
#define TIMEOUT			 -1
#define PATTERN_COUNT	 6
#define MAX_CLIENT_COUNT 3

// Parameters

#define USERLEN 18

// STRINGS

#define DOMAIN_NAME			 "ft_irc.local"
#define NETWORK_NAME		 "Server IRC"
#define SERVER_NAME			 "IRC"
#define END_MESSAGE			 "\r\n"
#define INVALID_NICK_CHARSET "#: \b\t\n\v\f\r"
#define INMES_PREFIX		 "<< "
#define OUTMES_PREFIX		 ">> "
#define DEFAULT_USERNAME	 "Placeholder"
#define SPECIAL_NICK_CHARSET "[]{}*\\|"

// COLORS

#define NC	   "\033[0m"
#define RED	   "\033[0;31m"
#define GREEN  "\033[0;32m"
#define ORANGE "\033[0;33m"
#define BLUE   "\033[0;34m"

// RPL

#define RPL_WELCOME "001 <client> :Welcome to the <networkname> Network, <nick>"

// Message

#define PONG_MESSAGE "PONG <servername> :<nick>"

// Sent Errors

#define ERR_CLOSECONNECTION	  "Connection closed"
#define ERR_UNKNOWNCOMMAND	  "421 <client> <command> :Unknown command"
#define ERR_NONICKNAMEGIVEN	  "431 <client> :No nickname given"
#define ERR_ERRONEUSNICKNAME  "432 <client> <arg> :Erroneus nickname"
#define ERR_NICKNAMEINUSE	  "433 <client> <arg> :Nickname is already in use"
#define ERR_NEEDMOREPARAMS	  "461 <client> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED "462 <client> :You may not reregister"
#define ERR_PASSWDMISMATCH	  "464 <client> :Password incorrect"

// Errors

#define ERROR_PREFIX		 "ERROR :"
#define LISTEN_FAIL__ERROR	 "listening failed"
#define READ_FAIL__ERROR	 "reading failed"
#define SEND_FAIL__ERROR	 "sending failed"
#define WRONG_CMD__ERROR	 "Invalid Login Command!"
#define CLOSED_CLIENT__ERROR "Client has been disconnected"

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
	void closeClient(Client *const client);

   private:
	// Attributes
	DataServ							   _socket;
	int									   _epollFd;
	std::map<std::string, CommandFunction> _cmdMap;
	std::string							   _password;
	ClientManager						   _clientMap;
	// Private Methods
	void processReceivedData(const std::string &received_data,
							 const int			clientFd);
	//    Send Methods
	void sendMessage(const std::string &message, const int clientFd) const;
	void sendFormattedMessage(const std::string	&message,
							  const Client *const client) const;
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
	void error(const std::string &message, Client *const client);
	// CMD_UTILS
	bool isNicknameAlreadyUsed(const std::string &nickname);
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
	class InvalidLoginCommandException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class ClosedClientException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};
