/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:09:28 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 12:09:47 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char const **av)
{
	char	buffer[1024] = { 0 };
	Data	data;

	if (ac == EXPECTED_ARG_COUNT)
	{
		std::cout << "Port is " << av[1] << std::endl;
		std::cout << "Password is " << av[2] << std::endl;
	}
	if (setUpServer(&data) == 1)
		return (EXIT_FAILURE);
	if (listen(data.servfd, MAX_CLIENT_COUNT) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (EXIT_FAILURE);
	}
	while (true)
	{
		if ((data.sockfd
			= accept(data.servfd, (struct sockaddr*)&data.address,
					&data.addrlen)) < 0) {
			std::cerr << "Error : while accepting" << std::endl;
			return (EXIT_FAILURE);
		}
		read(data.sockfd, buffer, 1024 - 1);	// subtract 1 for the null
												// terminator at the end
		std::cout << "Message received : " << buffer << std::endl;
		send(data.sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
		std::cout << "Hello message sent" << std::endl;
	}
	close(data.sockfd);
	close(data.servfd);
	return (EXIT_SUCCESS);
}
