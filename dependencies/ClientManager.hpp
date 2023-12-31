/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:53:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/14 15:48:01 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

#include "Client.hpp"
#include "Utils.hpp"

#define MAX_CLIENT_COUNT  100
#define SERVER_FULL_ERROR "Connection limit reached"

class ClientManager {
   public:
	ClientManager();
	~ClientManager();
	// Methods to change maps
	void addClient(Client* const client);
	void closeClient(Client* const client);
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
	// Attributes
	std::map<int, Client*>		   _socketToClientMap;
	std::map<std::string, Client*> _nicknameToClientMap;
	size_t						   _size;
	// Methods
	void eraseClient(Client* const client);
};
