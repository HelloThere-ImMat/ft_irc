/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 15:00:02 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>

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
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;
	void sendTopic(const Client *const client) const;

   private:
	// Attributes
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	bool								   _isTopicProtected;
	// Private methods
	bool isOp(const Client *const client) const;
};
