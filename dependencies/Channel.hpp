/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/11/30 01:51:44 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include "Client.hpp"

// DEBUGING
#include <iostream>

enum e_job {
	STANDARD,
	OPERATOR,
	ADMIN
};

class SpecifiedClient : public Client {
	public :
		SpecifiedClient() : Client(){}
		SpecifiedClient(Client *const ptr) : Client(*ptr) {}
		~SpecifiedClient() {}
	private:
		e_job	job;
		

};

class Channel {
	public :
		Channel(std::string name, Client *const client, const std::string &username);
		Channel() {}
		void addNewUser(Client *const client);
		void sendToChannel(std::string &username);
	private:
		std::string								_name;
		std::map<std::string,SpecifiedClient *>	_userMap;
};
