/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:09:28 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 16:10:29 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char **av)
{
	char	buffer[1024] = { 0 };

	if (ac != EXPECTED_ARG_COUNT)
		return (EXIT_SUCCESS);
	Server	serv(av[1]);

	std::cout << "Port is " << av[1] << std::endl;
	std::cout << "Password is " << av[2] << std::endl;

	if (serv.setup() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (listen(serv._socket.servfd, MAX_CLIENT_COUNT) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (EXIT_FAILURE);
	}
	while (true)
	{
		if ((serv._socket.sockfd
			= accept(serv._socket.servfd, (struct sockaddr*)&serv._socket.address,
					&serv._socket.addrlen)) < 0) {
			std::cerr << "Error : while accepting" << std::endl;
			return (EXIT_FAILURE);
		}
		read(serv._socket.sockfd, buffer, 1024 - 1);	// subtract 1 for the null
												// terminator at the end
		std::cout << "Message received : " << buffer << std::endl;
		send(serv._socket.sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
		std::cout << "Hello message sent" << std::endl;
	}
	close(serv._socket.sockfd);
	close(serv._socket.servfd);
	return (EXIT_SUCCESS);
}
