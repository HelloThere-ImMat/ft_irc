/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataServ.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:38 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 16:37:17 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#define DEFAULT_OPTION		1
#define SOCKET_INIT__ERROR	"Cannot create socket"
#define SOCKET_OPT__ERROR	"Invalid socket assets"
#define SOCKET_BIND__ERROR	"Cannot bind socket to address"
#define INVALID_PORT__ERROR	"Invalid port"
#define CANT_ACCEPT__ERROR	"Cannot accept client"

class DataServ
{
	public:

		DataServ(const std::string &port);
		DataServ(const int port);
		~DataServ();
		void	setup();
		int		acceptNewConnectionSocket();
		int		getSocketFd() const;

	private:
		// Attributes
		int 				_servfd;
		struct sockaddr_in	_address;
		socklen_t			_addrlen;
		// Private methods
		void	initAddress(const int port);
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
		class InvalidPortException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class CantAcceptException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};
