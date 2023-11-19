/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 17:59:07 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const char *const port): _socket(std::atoi(port)) {

	std::cout << "fd -> " << _socket.servfd << std::endl;
	this->option = 1;
}

Server::~Server() {}

#include <errno.h>

int	Server::setup() {

	try {
		if ((_socket.servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw SockInitException();
		if (setsockopt(_socket.servfd, SOL_SOCKET, SO_REUSEADDR, &(this->option), sizeof(option)) < 0)
			throw SockOptException();
		if (bind(_socket.servfd, (struct sockaddr*)&_socket.address, sizeof(_socket.address)) < 0)
			throw SockBindException();
	}
	catch(std::exception &e)
	{
		std::cerr << "Error : Setup : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

const char	*Server::SockInitException::what() const throw(){
	return (SOCKET_INIT__ERROR);
}

const char	*Server::SockOptException::what() const throw(){
	return (SOCKET_OPT__ERROR);
}

const char	*Server::SockBindException::what() const throw(){
	return (SOCKET_BIND__ERROR);
}
