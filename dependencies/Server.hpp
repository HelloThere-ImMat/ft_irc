/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/10 01:23:47 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <sys/epoll.h>

#include <exception>
#include <typeinfo>

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
#define MAX_CHANNEL_NB		100

// Parameters

#define USERLEN 18

// CHAR

#define SETTER_CHAR ':'

// STRINGS

#define INMES_PREFIX		 "<< "
#define CHANNEL_PREFIX		 '#'
#define DEFAULT_USERNAME	 "Placeholder"
#define SPECIAL_NICK_CHARSET "[]{}*\\|_"

// Logs

#define CLOSED_CLIENT_MESSAGE "Client has been disconnected"

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
	void invite(const std::vector<std::string> &cmd, Client *const client);
	void kick(const std::vector<std::string> &cmd, Client *const client);
	void error(const std::string &message, Client *const client);
	// CMD_UTILS
	bool isNicknameAlreadyUsed(const std::string &nickname);
	void sendJoinMessage(const Channel *const channel,
		const Client *const client, const std::string &channelName);
	void createChannel(
		const Client *const client, const std::string &channelName);
	void joinChannel(const std::vector<std::string> &cmd,
		const Client *const client, Channel *const channel,
		const size_t keyIndex);
	void sendPrivmsgToChannel(const Client *const client,
		const std::string &channelName, const std::string &privMessage);
	void sendPrivmsgToUser(Client *const client,
		const std::string &targetName, const std::string &privMessage);
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
	class InviteErrors : public std::exception {
		public:
			InviteErrors(const int errorCode) : _errorCode(errorCode) {}
			int getCode() const { return (_errorCode); }
		private:
			const int 		  _errorCode;
	};
};
