/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 10:31:54 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>

#include "Mode.hpp"
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
	void			   addNewUser(const Client *const client);
	void			   removeUser(const Client *const client);
	const std::string  getUserList() const;
	const std::string &getTopic() const;
	void			   setTopic(const std::string &topic);
	bool			   processMode(const std::vector<std::string> &cmd);
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	void sendTopic(const Client *const client) const;
	void sendTopicToAll(const Client *const client) const;
	void sendMode(const Client *const client) const;
	void sendModeToAll(const Client *const client) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;
	bool isOp(const Client *const client) const;

   private:
	// Attributes
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	bool								   _isTopicProtected;
	Mode								   _mode;
};
