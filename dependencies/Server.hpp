/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/14 14:50:06 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef TEST
	#define TEST false
#endif

#include <string.h>
#include <sys/epoll.h>

#include <typeinfo>

#include "Channel.hpp"
#include "ClientManager.hpp"
#include "DataServ.hpp"
#include "Signal.hpp"

// Count

#define BUFFER_SIZE			1024
#define TIMEOUT				-1
#define PRIVMSG_START_INDEX 2
#define TOPIC_START_INDEX	2
#define MODE_NOARG_SIZE		2
#define MIN_MODE_MSG_SIZE	3
#define MAX_CHANNEL_NB		10

// Parameters

#define USERLEN 18

// CHAR

#define CHANNEL_PREFIX '#'

// STRINGS

#define INMES_PREFIX			 "<< "
#define CHANNEL_PREFIX			 '#'
#define DEFAULT_USERNAME		 "Placeholder"
#define SPECIAL_NICK_CHARSET	 "[]{}*\\|_"
#define MODE_SETCHAR			 "itklo+-"
#define INVALID_PASSWORD_CHARSET " \b\t\n\v\f\r,"

// Errors

#define LISTEN_FAIL__ERROR			"listening failed"
#define READ_FAIL__ERROR			"reading failed"
#define CLIENT_QUIT__ERROR			"close connection"
#define WRONG_CMD__ERROR			"Invalid Login Command!"
#define INVALID_SET_PASSWORD__ERROR "Invalid set password"

typedef struct s_conv {
	const Client *user1;
	const Client *user2;
} Conversation;

class Server {
	typedef void (Server::*CommandFunction)(
		const std::vector<std::string> &, Client *const);

   public:
	Server(const std::string &port, const std::string &password);
	~Server();
	void listen() const;
	void lookForEvents();
	void readClientCommand(const int fd);
	void addNewClient();
	void closeClient(Client *const client, const std::string &quitMessage);

   private:
	// Attributes
	DataServ							   _socket;
	int									   _epollFd;
	std::map<std::string, Channel *>	   _channels;
	std::map<std::string, CommandFunction> _cmdMap;
	std::string							   _password;
	ClientManager						   _clientMap;
	std::vector<Conversation>			   _convList;
	// Private Methods
	//    Initialisation methods
	void initializeCmdMap();
	//    Print methods
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
	void removeUserFromChannels(const Client *const client);
	void removeUserConv(const Client *const client);
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
	void invite(const std::vector<std::string> &cmd, Client *const client);
	void kick(const std::vector<std::string> &cmd, Client *const client);
	void quit(const std::vector<std::string> &cmd, Client *const client);
	void mode(const std::vector<std::string> &cmd, Client *const client);
	void error(const std::string &message, Client *const client);
	// CMD_UTILS
	bool isNicknameAlreadyUsed(const std::string &nickname);
	void sendJoinMessage(const Channel *const channel,
		const Client *const client, const std::string &channelName);
	void createChannel(Client *const client, const std::string &channelName);
	void joinChannel(const std::vector<std::string> &cmd, Client *const client,
		Channel *const channel, const size_t keyIndex);
	void sendQuitMessageToOthers(
		const Client *const client, const std::string &quitMessage);
	void sendPrivmsgToChannel(const Client *const client,
		const std::string &channelName, const std::string &privMessage);
	void sendPrivmsgToUser(Client *const client, const std::string &targetName,
		const std::string &privMessage);
	void notifyInConv(
		const Client *const client, const std::string message) const;
	void notifyNickUpdate(
		const Client *const client, const std::string newNick) const;
	// Exceptions
	class ListenFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class ReadFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class ClientHasQuitException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class InvalidLoginCommandException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class InvalidSetPasswordException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
	class OpCmdsErrors : public std::exception {
	   public:
		OpCmdsErrors(const int errorCode) : _errorCode(errorCode) {}
		int getCode() const { return (_errorCode); }

	   private:
		const int _errorCode;
	};
};
