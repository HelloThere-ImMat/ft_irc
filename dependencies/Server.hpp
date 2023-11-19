/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 17:16:53 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Data.hpp"
#include "irc.hpp"
#include <sys/socket.h>
#include <exception>

#define SOCKET_INIT__ERROR	"creating socker"
#define SOCKET_OPT__ERROR	"setting sock options"
#define SOCKET_BIND__ERROR	"binding to address"

class Server {

	public:
		Server(const char *const port);
		~Server();
		int		setup();
		Data	_socket;
		int		option;


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
