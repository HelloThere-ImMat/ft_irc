/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 20:11:22 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Data.hpp"
#include "irc.hpp"
#include <string.h>

#define BUFFER_SIZE			1024
# define WELCOME_MESSAGE	"Hello from the Server"

class Server {

	public:
		Server(const int port, const std::string password);
		~Server();
		int	start();
		int	listen();
		int	acceptConnection();
		int	readMessage();
		int	sendMessage(const std::string &message);
	private:
		// Attributes
		Data		_socket;
		std::string	_password;
		char		buffer[BUFFER_SIZE];
};
