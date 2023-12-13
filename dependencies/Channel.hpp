/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/12 13:53:38 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

#include "Mode.hpp"
#include "Utils.hpp"

#define OP_PREFIX		   "@"
#define USERLIST_SEPARATOR " "
#define START_MODE_INDEX   3

struct SpecifiedClient {
	const Client *client;
	bool		  isOp;
};

class Channel {
   public:
	Channel(const std::string &name, const Client *const client);
	~Channel();
	void					 addNewUser(const Client *const client,
							const std::vector<std::string> &keys, const size_t keyIndex);
	void					 removeUser(const Client *const client);
	const std::string		 getUserList() const;
	const std::string		  &getTopic() const;
	const std::string		  &getName() const;
	void					 setTopic(const std::string &topic);
	std::vector<std::string> processMode(
		const std::vector<std::string> &cmd, Client *const client);
	void sendToOthers(
		const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	void sendTopic(const Client *const client) const;
	void sendTopicToAll(const Client *const client) const;
	void sendMode(const Client *const client) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;
	bool isOp(const Client *const client) const;
	bool isAbleToJoin(
		const std::vector<std::string> &cmd, const size_t passIndex) const;
	class WrongChannelKeyException : public std::exception {};
	class TooManyUserException : public std::exception {};
	class UserNotInChannelException : public std::exception {};

   private:
	// Attributes
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	Mode								   _mode;
	// Private methods
	bool tryModeApplication(const t_modSetter setter, const char cflag,
		std::string arg, Client *const client);
	void setModeParameter(const char c, std::string &arg,
		const t_modSetter setter, Client *const client);
	bool isFull() const;
};
