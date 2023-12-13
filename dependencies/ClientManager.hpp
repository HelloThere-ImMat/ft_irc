/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:53:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/13 10:24:06 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

#include "Client.hpp"
#include "Utils.hpp"
// debug
#include <iostream>

class ClientManager {
   public:
	ClientManager();
	~ClientManager();
	// Methods to change maps
	void addClient(Client* const client);
	void eraseClient(Client* const client);
	void sendToAllClients(const std::string &message, Client *const client);
	void updateClientNickname(
		Client* const client, const std::string& nickname);
	// Methods to retrieve a client based on either socket or nickname
	Client* getClient(const std::string& nickname);
	Client* getClient(const int sockFd);

   private:
	std::map<int, Client*>		   _socketToClientMap;
	std::map<std::string, Client*> _nicknameToClientMap;
};
