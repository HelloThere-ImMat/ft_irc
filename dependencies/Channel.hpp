/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 00:59:20 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>

#define EXIT_FAILURE 1
#define EXIT_SUCESS	 0

#include "SendCmd.hpp"

#define OP_PREFIX		   "@"
#define USERLIST_SEPARATOR " "

struct SpecifiedClient {
	const Client *client;
	bool		  isOp;
};

class Channel {
   public:
	Channel(const std::string &name, const Client *const client);
	~Channel();
	int	 addNewUser(const Client *const client, std::vector<std::string> &keys,
		 size_t keyIndex);
	void removeUser(const Client *const client);
	const std::string  getUserList() const;
	const std::string &getTopic() const;
	void			   setTopic(const std::string &topic);
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	void sendTopic(const Client *const client) const;
	void sendTopicToAll(const Client *const client) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;

   private:
	// Attributes
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	std::string							   _password;
	bool								   _isTopicProtected;
	bool								   _isPasswordProtected;
	// Private methods
	bool isOp(const Client *const client) const;
};
