/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/08 14:55:02 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

#include "Utils.hpp"

#define OP_PREFIX		   "@"
#define USERLIST_SEPARATOR " "

#define MAX_USER_PER_CHANNEL 15

struct SpecifiedClient {
	const Client *client;
	bool		  isOp;
};

class Channel {
   public:
	Channel(const std::string &name, const Client *const client);
	~Channel();
	void			   addNewUser(const Client *const client,
					  const std::vector<std::string> &keys, const size_t keyIndex);
	void			   removeUser(const Client *const client);
	const std::string  getUserList() const;
	const std::string &getTopic() const;
	const std::string &getName() const;
	void			   setTopic(const std::string &topic);
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	void sendTopic(const Client *const client) const;
	void sendTopicToAll(const Client *const client) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;
	class WrongChannelKeyException : public std::exception {};
	class TooManyUserException : public std::exception {};

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
