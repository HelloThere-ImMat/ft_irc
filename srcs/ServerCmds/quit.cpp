/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:17:28 by mat               #+#    #+#             */
/*   Updated: 2023/12/14 16:20:20 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendQuitMessageToOthers(
	const Client *const client, const std::string &quitMessage) {
	client->sendToChannels(quitMessage);
}

void Server::quit(const std::vector<std::string> &cmd, Client *const client) {
	const std::string quitMessage = Utils::getFullMessage(cmd, 0);
	error(quitMessage, client);
}
