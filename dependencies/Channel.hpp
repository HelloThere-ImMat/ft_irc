/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/05 14:56:54 by rbroque          ###   ########.fr       */
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
	void			   setTopic(const std::string &topic);
	void			   sendToOthers(
					  const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	bool userIsInChannel(const Client *const client);

   private:
	std::map<std::string, SpecifiedClient> userMap;
	std::string							   _topic;
	const std::string					   _name;
};
