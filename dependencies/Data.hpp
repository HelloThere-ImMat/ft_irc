/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:38 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 20:10:04 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <exception>

#define DEFAULT_OPTION		1
#define SOCKET_INIT__ERROR	"creating socker"
#define SOCKET_OPT__ERROR	"setting sock options"
#define SOCKET_BIND__ERROR	"binding to address"

class Data
{
	public:

		Data(const int port);
		~Data();
		int		setup();
		int		acceptConnection();
		int		getSocketFd() const;
		int		getServerSocketFd() const;
		void	setSocketFd(const int newFd);

	private:
		// Attributes
		int					sockfd;
		int 				servfd;
		struct sockaddr_in	address;
		socklen_t			addrlen;
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
