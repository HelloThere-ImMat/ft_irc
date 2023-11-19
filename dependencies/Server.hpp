/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 18:25:30 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Data.hpp"
#include "irc.hpp"
#include <sys/socket.h>
#include <exception>

#define BUFFER_SIZE			1024
#define SOCKET_INIT__ERROR	"creating socker"
#define SOCKET_OPT__ERROR	"setting sock options"
#define SOCKET_BIND__ERROR	"binding to address"

class Server {

	public:
		Server(const char *const port);
		~Server();
		int		setup();
		int		listen();
		int		acceptConnection();
		int		readMessage();
		int		sendMessage(const std::string &message);
		Data	_socket;
		int		option;
		char	buffer[BUFFER_SIZE];
	private:
		// Exceptions
		class SockInitException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class SockOptException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class SockBindException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};
