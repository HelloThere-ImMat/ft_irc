/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:55:46 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 11:50:20 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>

#include <iostream>
#include <sstream>

#include "Client.hpp"

// COLORS

#define NC	   "\033[0m"
#define RED	   "\033[0;31m"
#define GREEN  "\033[0;32m"
#define ORANGE "\033[0;33m"
#define BLUE   "\033[0;34m"
#define GREY   "\033[0;90m"

// Count

#define PATTERN_COUNT	 9

// Server specs

#define HOST_NAME	 "localhost"
#define NETWORK_NAME "IRC"
#define SERVER_NAME	 "IRCserv"
#define DOMAIN_NAME	 "ft_irc.local"
#define END_MESSAGE	 "\r\n"

#define OUTMES_PREFIX ">> "

// Error

#define SEND_FAIL__ERROR "sending failed"

class SendCmd
{
	public :
		static void sendMessage(const std::string &message, const int clientFd);
		static void sendPrivateMessage(const std::string &message, const Client *const sender, const Client *const receiver);
		static void sendFormattedMessage(
			const std::string &message, const Client *const client, std::string channelName = "");
	private :
		class SendFailException : public std::exception {
		public:
			virtual const char *what() const throw();
		};
};
