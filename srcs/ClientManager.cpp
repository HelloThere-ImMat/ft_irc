/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdorr <mdorr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:53:03 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/13 10:26:12 by mdorr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager() {
	for (std::map<int, Client*>::iterator it = _socketToClientMap.begin();
		 it != _socketToClientMap.end(); ++it) {
		eraseClient(it->second);
		delete it->second;
	}
}

void ClientManager::addClient(Client* const client) {
	_socketToClientMap[client->getSocketFd()] = client;
	updateClientNickname(client, client->getNickname());
}

void ClientManager::updateClientNickname(
	Client* const client, const std::string& nickname) {
	if (nickname.empty() == false) {
		const std::string oldNickname = client->getNickname();
		if (oldNickname.empty() == false)
			_nicknameToClientMap.erase(oldNickname);
		_nicknameToClientMap[nickname] = client;
	}
}

void ClientManager::eraseClient(Client* const client) {
	const std::string nickname = client->getNickname();

	if (nickname.empty() == false)
		_nicknameToClientMap.erase(nickname);
}

void ClientManager::sendToAllClients(const std::string &message, Client *const client)
{
	for (std::map<int, Client *>::iterator it = _socketToClientMap.begin(); it != _socketToClientMap.end(); it++)
		Utils::sendPrivateMessage(message, client, it->second);
}

Client* ClientManager::getClient(const std::string& nickname) {
	if (_nicknameToClientMap.find(nickname) != _nicknameToClientMap.end()) {
		return _nicknameToClientMap[nickname];
	}
	return NULL;
}

Client* ClientManager::getClient(const int sockFd) {
	if (_socketToClientMap.find(sockFd) != _socketToClientMap.end()) {
		return _socketToClientMap[sockFd];
	}
	return NULL;
}
