/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 08:35:31 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/08 17:59:21 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#define DEFAULT_OPTION		1
#define MAX_PORT_VALUE		65535
#define SOCKET_INIT__ERROR	"Cannot create socket"
#define SOCKET_OPT__ERROR	"Invalid socket assets"
#define SOCKET_BIND__ERROR	"Cannot bind socket to address"
#define INVALID_PORT__ERROR "Invalid port"
#define CANT_ACCEPT__ERROR	"Cannot accept client"

class DataServ {
   public:
	explicit DataServ(const std::string& port);
	explicit DataServ(const int port);
	~DataServ();
	void setup();
	int	 acceptNewConnectionSocket();
	int	 getSocketFd() const;

   private:
	// Attributes
	int				   _servfd;
	socklen_t		   _addrlen;
	struct sockaddr_in _address;
	// Private methods
	void initAddress(const int port);
	// Exceptions
	class SockInitException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};
	class SockOptException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};
	class SockBindException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};
	class InvalidPortException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};
	class CantAcceptException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};
};
