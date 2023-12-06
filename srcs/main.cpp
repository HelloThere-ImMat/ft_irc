/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:09:28 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/06 09:42:00 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static int startServer(const std::string &port, const std::string &password) {
	try {
		Server serv(port, password);

		serv.listen();
		while (true) {
			serv.lookForEvents();
		}
	} catch (std::exception &e) {
		if (typeid(e) == typeid(Signal::ExitException))
			std::cerr << e.what() << std::endl;
		else
			std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char **av) {
	if (ac != EXPECTED_ARG_COUNT)
		return (EXIT_FAILURE);
	signal(SIGINT, Signal::exit_signal);
	signal(SIGQUIT, Signal::exit_signal);
	return (startServer(av[1], av[2]));
}
