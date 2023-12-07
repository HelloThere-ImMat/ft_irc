/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/07 00:05:00 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <vector>

#include "SendCmd.hpp"

#define OP_PREFIX		   "@"
#define USERLIST_SEPARATOR " "

#define NO_MOD			  0x00
#define INVITE_ONLY		  0x01
#define TOPIC_RESTRICTION 0x02
#define PASS_ONLY		  0x04
#define USERLIMIT		  0x08

typedef enum e_modSetter { ADD, RM } t_modSetter;

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
	void			   setFlags(const uint8_t flags, const t_modSetter setter);
	bool			   setMode(const t_modSetter setter, const char c,
					  std::vector<std::string> &modArgs);
	bool			   processMode(
					  const std::vector<std::string> &cmd, const Client *const client);
	void sendToOthers(
		const Client *const client, const std::string message) const;
	void sendToAll(const Client *const client, const std::string message) const;
	void sendTopic(const Client *const client) const;
	void sendTopicToAll(const Client *const client) const;
	void sendMode(const Client *const client) const;
	void sendModeToAll(const Client *const client) const;
	bool isUserInChannel(const Client *const client) const;
	bool canChangeTopic(const Client *const client) const;

   private:
	// Attributes
	std::map<std::string, SpecifiedClient> userMap;
	const std::string					   _name;
	std::string							   _topic;
	bool								   _isTopicProtected;
	uint8_t								   _modMask;
	// Private methods
	std::string getModeMessage() const;
	bool		isOp(const Client *const client) const;
};
