/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:17:28 by mat               #+#    #+#             */
/*   Updated: 2023/12/14 14:36:51 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendQuitMessageToOthers(
	const Client *const client, const std::string &quitMessage) {
	for (std::map<std::string, Channel *>::iterator it = _channels.begin();
		 it != _channels.end(); it++) {
		const Channel *const channel = it->second;
		if (channel->isUserInChannel(client))
			channel->sendToOthers(client, quitMessage);
	}
	// TO BE UPDATED
}

void Server::quit(const std::vector<std::string> &cmd, Client *const client) {
	const std::string quitMessage = Utils::getFullMessage(cmd, 0);
	error(quitMessage, client);
}
