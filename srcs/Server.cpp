/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 17:11:51 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const char *const port): _socket(std::atoi(port)) {

	this->option = 1;
}

Server::~Server() {}

int	Server::setup() {

	//Setting address to local host

	try {
		if ((_socket.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw SockInitException();
		std::cout << "->   creating socket done!" << std::endl;
		if (setsockopt(_socket.servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
			throw SockOptException();
		std::cout << "->   setting socket option done!" << std::endl;
		if (bind(_socket.servfd, (struct sockaddr*)&_socket.address, sizeof(_socket.address)) < 0)
			throw SockBindException();
		std::cout << "->   binding done!" << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << "Error : Setup : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

const char	*Server::SockInitException::what() const throw(){
	return (SOCKET_CREATE__ERROR);
}

const char	*Server::SockOptException::what() const throw(){
	return (SOCKET_SETTING__ERROR);
}

const char	*Server::SockBindException::what() const throw(){
	return (SOCKET_BINDING__ERROR);
}
