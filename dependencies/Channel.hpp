/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 14:56:40 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include "Client.hpp"

// DEBUGING
#include <iostream>

enum e_job {
	STANDARD,
	OPERATOR
};

class SpecifiedClient {
	public :
		SpecifiedClient(Client *const ptr, e_job job) : clientRef(ptr), _job(job) {}
		~SpecifiedClient() {}
		Client	*clientRef;
	private:
		SpecifiedClient() {}
		e_job	_job;
		

};

class Channel {
	public :
		Channel(std::string name, Client *const client, const std::string &nickname);
		Channel() {}
		void addNewUser(Client *const client);
		void sendToChannel(std::string &nickname);
	private:
		std::string								_name;
		std::map<std::string,SpecifiedClient *>	_userMap;
};
