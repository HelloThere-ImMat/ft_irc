/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:37:08 by mdorr             #+#    #+#             */
/*   Updated: 2023/12/14 17:46:28 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstdlib>
#include <ctime>

#include "Utils.hpp"

#define LOADING_RESP_NB 	5
#define BOT_TIME_LOAD		1
#define BOT_TIME_SUSPENSE	2

#define BOT_INTRO_MESS	":Hi <client>,"
#define BOT_INTRO_MESS2 "My name is Marvin, I am a simple script bot."
#define BOT_INTRO_MESS3                                                   \
	"I am based of a Novel from Douglas Adams called \"The Hitchhiker's " \
	"Guide to the Galaxy\"."
#define BOT_INTRO_MESS4 \
	"You can ask me anything you want, i will be happy to try and help you"

#define BOT_Q_ANS	  "I don't answer stupid questions"
#define BOT_BASIC_ANS "F*ck you"

class Bot {
   public:
	Bot();
	void interact(const Client *const client, const std::string &message);
	const std::string &getName() const;
	void			   removeUserFromList(const Client *const client);
	~Bot() {}

   private:
	std::string					_name;
	std::vector<const Client *> _botUsers;
	void sendMessage(const Client *const client, const char *const message) const;
	void respond(const Client *const client, const std::string &message) const;
	void sendIntroMessage(const Client *const client) const;
};