/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:09:28 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 18:37:17 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static int startServer(const std::string &port, const std::string &password) {
	try {
		Server serv(port, password);

		serv.start();
		serv.listen();
		while (true) {
			serv.lookForEvents();
		}
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char **av) {
	if (ac != EXPECTED_ARG_COUNT)
		return (EXIT_SUCCESS);
	return (startServer(av[1], av[2]));
}
