/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:15:21 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 19:03:25 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"

Data::Data(const int port) {

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
}

int	Data::setup() {

	const int	option = DEFAULT_OPTION;

	try {
		if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw SockInitException();
		if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
			throw SockOptException();
		if (bind(servfd, (struct sockaddr*)&address, sizeof(address)) < 0)
			throw SockBindException();
	}
	catch(std::exception &e)
	{
		std::cerr << "Error : Setup : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Data::acceptConnection() {

	sockfd = accept(servfd,
						(struct sockaddr*)&address, &addrlen);
	if (sockfd < 0)
	{
		std::cerr << "Error : while accepting" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	Data::getSocketFd() const {
	return (sockfd);
}

int	Data::getServerSocketFd() const {
	return (servfd);
}

void	Data::setSocketFd(const int newFd) {
	sockfd = newFd;
}

Data::~Data() {
	close(sockfd);
	close(servfd);
}

/////////////
// PRIVATE //
/////////////

const char	*Data::SockInitException::what() const throw(){
	return (SOCKET_INIT__ERROR);
}

const char	*Data::SockOptException::what() const throw(){
	return (SOCKET_OPT__ERROR);
}

const char	*Data::SockBindException::what() const throw(){
	return (SOCKET_BIND__ERROR);
}
