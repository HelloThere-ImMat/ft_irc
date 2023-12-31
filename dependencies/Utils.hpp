/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 23:58:57 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/14 17:31:02 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>

#include <iostream>
#include <sstream>
#include <vector>

#include "Client.hpp"

// COLORS

#define NC	   "\033[0m"
#define RED	   "\033[0;31m"
#define GREEN  "\033[0;32m"
#define ORANGE "\033[0;33m"
#define BLUE   "\033[0;34m"
#define GREY   "\033[0;90m"

// Count

#define PATTERN_COUNT 9

// Char

#define SETTER_CHAR ':'

// Name

#define BOT_NAME "marvin"

// Server specs

#define HOST_NAME	 "localhost"
#define NETWORK_NAME "IRC"
#define SERVER_NAME	 "IRCserv"
#define DOMAIN_NAME	 "ft_irc.local"
#define END_MESSAGE	 "\r\n"

#define OUTMES_PREFIX	  ">> "
#define CMD_ARG_SEPARATOR ','

// RPL

#define RPL_WELCOME		  "001 <client> :Welcome to the <networkname> Network, <nick>"
#define RPL_CHANNELMODEIS "324 <client> <channelName> "
#define RPL_NOTOPIC		  "331 <client> <channelName> :No topic is set"
#define RPL_TOPIC		  "332 <client> <channelName> :"
#define UL_JOIN_MESSAGE	  "353 <nick> = <channelName> :"
#define EUL_JOIN_MESSAGE  "366 <client> <channelName> :End of /NAMES list."
#define RPL_INVITING	  "341 <client> <arg> <channelName>"

// Message

#define DEFAULT_QUIT	  "QUIT :leaving"
#define MESSAGE_SEPARATOR " "
#define JOIN_PREFIX		  "JOIN :"
#define PRIVMSG_PREFIX	  "PRIVMSG "
#define PART_PREFIX		  "PART "
#define NICK_PREFIX		  "NICK :"
#define INVITATION		  "INVITE <nick> <channelName>"
#define KICK			  "KICK <channelName> <arg> :"
#define PONG_MESSAGE	  "PONG <servername> :<nick>"

// Sent Errors

#define ERROR_PREFIX		"ERROR :"
#define ERR_CLOSECONNECTION "Connection closed"
#define ERR_NOSUCHNICK		"401 <client> <arg> :No such nick/channel"
#define ERR_NOSUCHCHANNEL	"403 <client> <channelName> :No such channel"
#define ERR_CANNOTSENDTOCHAN \
	"404 <client> <channelName> :Cannot send to channel"
#define ERR_TOOMANYCHANNELS \
	"405 <client> <channelName> :You have joined too many channels"
#define ERR_NOTEXTTOSEND	 "412 <client> :No text to send"
#define ERR_UNKNOWNCOMMAND	 "421 <client> <command> :Unknown command"
#define ERR_NONICKNAMEGIVEN	 "431 <client> :No nickname given"
#define ERR_ERRONEUSNICKNAME "432 <client> <arg> :Erroneus nickname"
#define ERR_NICKNAMEINUSE	 "433 *<client> <arg> :Nickname is already in use"
#define ERR_USERNOTINCHANNEL \
	"441 <client> <arg> <channelName> :They aren't on that channel"
#define ERR_NOTONCHANNEL \
	"442 <client> <channelName> :You're not on that channel"
#define ERR_USERONCHANNEL \
	"443 <client> <arg> <channelName> :is already on channel"
#define ERR_NEEDMOREPARAMS	  "461 <client> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED "462 <client> :You may not reregister"
#define ERR_PASSWDMISMATCH	  "464 <client> :Password incorrect"
#define ERR_CHANNELISFULL	  "471 <client> <channelName> :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN \
	"473 <client> <channelName> :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY "475 <client> <channelName> :Cannot join channel (+k)"
#define ERR_BADCHANMASK	  "476 <channelName> :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED \
	"482 <client> <channelName> :You're not channel operator"

// Error

#define SEND_FAIL_ERROR "sending failed"

class Bot;

class Utils {
   public:
	static void sendMessage(
		const std::string &message, const Client *const client);
	static void sendPrivateMessage(const std::string &message,
		const Client *const sender, const Client *const receiver);
	static void sendPrivateMessage(const std::string &message, const Bot &bot,
		const Client *const receiver);
	static void sendFormattedMessage(const std::string &message,
		const Client *const client, const std::string channelName = "");
	static std::string getFormattedMessage(const std::string &message,
		const Client *const client, const std::string channelName = "");
	static std::vector<std::string> splitString(
		const std::string &listStr, const char delimiter);
	static std::string getFullMessage(
		const std::vector<std::string> &cmd, const size_t startIndex);
	static void removeDuplicateChars(std::string &str);
	static bool isSetter(const char c);
	static bool isThereInvalidChar(
		const std::string &str, const std::string &invalidCharset);
	static std::string removeSetterChar(const std::string &message);

   private:
	static void sendPrivateMessage(const std::string &message,
		const std::string &senderNick, const std::string &senderName,
		const Client *const target);
	class SendFailException : public std::exception {
	   public:
		virtual const char *what() const throw();
	};
};
