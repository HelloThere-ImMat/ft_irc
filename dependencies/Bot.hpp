/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:37:08 by mdorr             #+#    #+#             */
/*   Updated: 2023/12/14 15:32:19 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Utils.hpp"

#define BOT_INTRO_MESS ":Hi <client>,"
#define BOT_INTRO_MESS2 "My name is Marvin, I am a simple script bot."
#define BOT_INTRO_MESS3 "I am based of a Novel from <…> called <…>."
#define BOT_INTRO_MESS4 "You can ask me anything you want, i will be happy to try and help you"

class Bot
{
	public:
		Bot();
		void 		interact(const Client *const client);
		void		sendMessage(const Client *const client, const char *message);
		const std::string &getName() const;
		void		removeUserFromList(const Client *const client);
		~Bot() {}
	private:
		std::string					_name;
		std::vector<const Client *> _botUsers;
		std::vector<std::string>	_loadingResponses;
};