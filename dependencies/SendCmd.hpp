/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:55:46 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 15:22:12 by rbroque          ###   ########.fr       */
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

#define PATTERN_COUNT 8

// Server specs

#define HOST_NAME	 "localhost"
#define NETWORK_NAME "IRC"
#define SERVER_NAME	 "IRCserv"
#define DOMAIN_NAME	 "ft_irc.local"
#define END_MESSAGE	 "\r\n"

#define OUTMES_PREFIX ">> "

// RPL

#define RPL_WELCOME "001 <client> :Welcome to the <networkname> Network, <nick>"
#define RPL_NOTOPIC "331 <client> <arg> :No topic is set"
#define RPL_TOPIC	"332 <client> <arg> :"

// Message

#define JOIN_PREFIX		 "JOIN :"
#define PRIVMSG_PREFIX	 "PRIVMSG "
#define PART_PREFIX		 "PART "
#define UL_JOIN_MESSAGE	 "353 <nick> = <arg> :"
#define EUL_JOIN_MESSAGE "366 <client> <arg> :End of /NAMES list."
#define PONG_MESSAGE	 "PONG <servername> :<nick>"

// Sent Errors

#define ERROR_PREFIX		  "ERROR :"
#define ERR_CLOSECONNECTION	  "Connection closed"
#define ERR_NOSUCHCHANNEL	  "403 <client> <arg> :No such channel"
#define ERR_UNKNOWNCOMMAND	  "421 <client> <command> :Unknown command"
#define ERR_NONICKNAMEGIVEN	  "431 <client> :No nickname given"
#define ERR_ERRONEUSNICKNAME  "432 <client> <arg> :Erroneus nickname"
#define ERR_NICKNAMEINUSE	  "433 *<client> <arg> :Nickname is already in use"
#define ERR_NOTONCHANNEL	  "442 <client> <arg> :You're not on that channel"
#define ERR_NEEDMOREPARAMS	  "461 <client> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED "462 <client> :You may not reregister"
#define ERR_PASSWDMISMATCH	  "464 <client> :Password incorrect"
#define ERR_CHANOPRIVSNEEDED  "482 <client> <arg> :You're not channel operator"

// Error

#define SEND_FAIL__ERROR "sending failed"

class SendCmd {
   public:
	static void sendMessage(
		const std::string &message, const Client *const client);
	static void sendPrivateMessage(const std::string &message,
		const Client *const sender, const Client *const receiver);
	static void sendFormattedMessage(
		const std::string &message, const Client *const client);
	static std::string getFormattedMessage(
		const std::string &message, const Client *const client);

   private:
	class SendFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};
