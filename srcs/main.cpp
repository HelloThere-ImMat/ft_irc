/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:09:28 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 20:07:51 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char **av)
{
	if (ac != EXPECTED_ARG_COUNT)
		return (EXIT_SUCCESS);
	Server	serv(std::atoi(av[1]), av[2]);

	if (serv.start() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (serv.listen() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (true)
	{
		if (serv.acceptConnection() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (serv.readMessage() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (serv.sendMessage(WELCOME_MESSAGE) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
