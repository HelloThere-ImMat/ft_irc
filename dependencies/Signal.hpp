/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 21:54:14 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/27 16:08:44 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <signal.h>

#include <exception>

#define EXIT_SIGNAL "Exited by signal"

class Signal {
   public:
	static void exit_signal(int signum);
	class ExitException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};