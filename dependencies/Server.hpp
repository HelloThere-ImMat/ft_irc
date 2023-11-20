/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 17:15:01 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Data.hpp"
#include "irc.hpp"

#define BUFFER_SIZE			1024
#define WELCOME_MESSAGE		"Hello from the Server\n"
#define LISTEN_FAIL__ERROR	"listening failed"
#define READ_FAIL__ERROR	"reading failed"
#define SEND_FAIL__ERROR	"sending failed"

class Server {

	public:
		Server(const std::string &port, const std::string &password);
		~Server();
		void	start();
		void	listen() const;
		void	acceptConnection();
		void	sendMessage(const std::string &message) const;
		void	readMessage();
	private:
		// Attributes
		Data		_socket;
		std::string	_password;
		// Exceptions
		class ListenFailException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class ReadFailException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class SendFailException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};
