/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 21:52:11 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/26 23:01:52 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Signal.hpp"

void Signal::handle_sigint(int signum) {
	if (signum == SIGINT)
		throw ExitException();
}

void Signal::handle_sigquit(int signum) {
	if (signum == SIGQUIT)
		throw ExitException();
}

const char *Signal::ExitException::what() const throw() {
	return (EXIT_SIGNAL);
}