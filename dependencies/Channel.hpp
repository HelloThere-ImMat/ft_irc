/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/06 09:25:44 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>

#include "SendCmd.hpp"

#define OP_PREFIX "@"

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
	const std::string &getName() const;
	bool			   getTopicPermission() const;
	void			   setTopic(const std::string &topic);
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	bool isUserInChannel(const Client *const client) const;
	bool isOp(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;
	void sendTopic(const Client *const client) const;

   private:
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	bool								   _topicPermission;
};
