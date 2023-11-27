/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 21:52:11 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/27 16:09:35 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Signal.hpp"

void Signal::exit_signal(int signum) {
	if (signum == SIGINT || signum == SIGQUIT)
		throw ExitException();
}

const char *Signal::ExitException::what() const throw() {
	return (EXIT_SIGNAL);
}