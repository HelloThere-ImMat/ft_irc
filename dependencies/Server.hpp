/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/23 16:53:26 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "DataServ.hpp"
#include "irc.hpp"

#define BUFFER_SIZE 1024
#define TIMEOUT -1
#define MAX_CLIENT_COUNT 3
#define WELCOME_MESSAGE "Hello from the Server\n"
#define LISTEN_FAIL__ERROR "listening failed"
#define READ_FAIL__ERROR "reading failed"
#define SEND_FAIL__ERROR "sending failed"
#define END_MESSAGE "\r\n"

class Server {
   public:
	Server(const std::string &port, const std::string &password);
	~Server();
	void start();
	void listen() const;
	void lookForEvents();
	void sendMessage(const std::string &message) const;
	void readClientCommand(const int fd);
	void addNewClient();

   private:
	// Attributes
	DataServ _socket;
	Client _client;
	int _epollFd;
	std::string _password;
	// Private Methods
	void addFdToPoll(const int fd);
	void delFdToPoll(const int fd);
	void processReceivedData(const std::string &received_data);
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
};
