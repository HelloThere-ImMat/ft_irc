/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:53:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/13 11:17:03 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <map>

#include "Client.hpp"
#include "Utils.hpp"

#define MAX_CLIENT_COUNT  20
#define SERVER_FULL_ERROR "Connection limit reached"

class ClientManager {
   public:
	ClientManager();
	~ClientManager();
	// Methods to change maps
	void addClient(Client* const client);
	void eraseClient(Client* const client);
	void sendToAllClients(const std::string& message, Client *const client);
	void updateClientNickname(
		Client* const client, const std::string& nickname);
	// Methods to retrieve a client based on either socket or nickname
	Client* getClient(const std::string& nickname);
	Client* getClient(const int sockFd);
	// Exceptions
	class ServerFullException : public std::exception {
	   public:
		virtual const char* what() const throw();
	};

   private:
	std::map<int, Client*>		   _socketToClientMap;
	std::map<std::string, Client*> _nicknameToClientMap;
	size_t						   _size;
};
