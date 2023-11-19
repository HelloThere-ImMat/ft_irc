/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:38 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 21:40:51 by rbroque          ###   ########.fr       */
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

class Data
{
	public:

		Data(const std::string &port);
		Data(const int port);
		~Data();
		void	setup();
		void	acceptConnection();
		void	setSocketFd(const int newFd);
		int		getSocketFd() const;
		int		getServerSocketFd() const;

	private:
		// Attributes
		int					sockfd;
		int 				servfd;
		struct sockaddr_in	address;
		socklen_t			addrlen;
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
