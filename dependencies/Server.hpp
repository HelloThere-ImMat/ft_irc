/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/05 16:58:32 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <sys/epoll.h>

#include <exception>
#include <typeinfo>
#include <vector>

#include "Channel.hpp"
#include "ClientManager.hpp"
#include "DataServ.hpp"
#include "Signal.hpp"

// Count

#define BUFFER_SIZE			1024
#define TIMEOUT				-1
#define MAX_CLIENT_COUNT	3
#define PRIVMSG_START_INDEX 2
#define TOPIC_START_INDEX	2

// Parameters

#define USERLEN 18

// STRINGS

#define INMES_PREFIX		 "<< "
#define DEFAULT_USERNAME	 "Placeholder"
#define SPECIAL_NICK_CHARSET "[]{}*\\|_"

// RPL

#define RPL_WELCOME "001 <client> :Welcome to the <networkname> Network, <nick>"
#define RPL_TOPIC	"332 <client> <arg> "

// Message

#define JOIN_PREFIX		 "JOIN :"
#define PRIVMSG_PREFIX	 "PRIVMSG "
#define PART_PREFIX		 "PART "
#define UL_JOIN_MESSAGE	 "353 <nick> = <arg> :"
#define EUL_JOIN_MESSAGE "366 <client> <arg> :End of /NAMES list."
#define PONG_MESSAGE	 "PONG <servername> :<nick>"

// Logs

#define CLOSED_CLIENT_MESSAGE "Client has been disconnected"

// Sent Errors

#define ERROR_PREFIX		  "ERROR :"
#define ERR_CLOSECONNECTION	  "Connection closed"
#define ERR_UNKNOWNCOMMAND	  "421 <client> <command> :Unknown command"
#define ERR_NONICKNAMEGIVEN	  "431 <client> :No nickname given"
#define ERR_ERRONEUSNICKNAME  "432 <client> <arg> :Erroneus nickname"
#define ERR_NICKNAMEINUSE	  "433 *<client> <arg> :Nickname is already in use"
#define ERR_NEEDMOREPARAMS	  "461 <client> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED "462 <client> :You may not reregister"
#define ERR_PASSWDMISMATCH	  "464 <client> :Password incorrect"

// Errors

#define LISTEN_FAIL__ERROR "listening failed"
#define READ_FAIL__ERROR   "reading failed"
#define WRONG_CMD__ERROR   "Invalid Login Command!"

class Server {
	typedef void (Server::*CommandFunction)(
		const std::vector<std::string> &, Client *const);

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
	std::map<std::string, Channel *>	   _channels;
	std::map<std::string, CommandFunction> _cmdMap;
	std::string							   _password;
	ClientManager						   _clientMap;
	// Private Methods
	void printLog(const std::string &logMessage) const;
	//    Poll Methods
	void addFdToPoll(const int fd);
	void delFdToPoll(const int fd);
	//    Cmd Handling Methods
	void processReceivedData(
		const std::string &received_data, const int clientFd);
	void handleClientMessage(const std::string &message, Client *const client);
	void handleCmd(const std::vector<std::string> &cmd, Client *const client);
	void getUserLogin(
		const std::vector<std::string> &cmd, Client *const client);
	//    Log Methods
	void tryPasswordAuth(
		const std::vector<std::string> &cmd, Client *const client);
	void setClientLogAssets(
		const std::vector<std::string> &cmd, Client *const client);
	// CMD
	void cap(const std::vector<std::string> &cmd, Client *const client);
	void pass(const std::vector<std::string> &cmd, Client *const client);
	void user(const std::vector<std::string> &cmd, Client *const client);
	void nick(const std::vector<std::string> &cmd, Client *const client);
	void ping(const std::vector<std::string> &cmd, Client *const client);
	void join(const std::vector<std::string> &cmd, Client *const client);
	void privmsg(const std::vector<std::string> &cmd, Client *const client);
	void part(const std::vector<std::string> &cmd, Client *const client);
	void topic(const std::vector<std::string> &cmd, Client *const client);
	void error(const std::string &message, Client *const client);
	// CMD_UTILS
	bool isNicknameAlreadyUsed(const std::string &nickname);
	void sendJoinMessage(const Channel *const channel,
		const Client *const client, const std::string &channelName);
	// Exceptions
	class ListenFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class ReadFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class InvalidLoginCommandException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};
