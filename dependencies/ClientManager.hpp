/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:53:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/01 16:47:10 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

#include "Client.hpp"
//debug
#include <iostream>

class ClientManager {
   public:
	ClientManager();
	~ClientManager();
	// Methods to change maps
	void addClient(Client* const client);
	void eraseClient(Client* const client);
	void updateClientNickname(
		Client* const client, const std::string& nickname);
	// Methods to retrieve a client based on either socket or nickname
	Client* getClient(const std::string& nickname);
	Client* getClient(const int sockFd);
	//debug
	void 	printUserList();

   private:
	std::map<int, Client*>		   _socketToClientMap;
	std::map<std::string, Client*> _nicknameToClientMap;
};
