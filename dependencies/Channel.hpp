/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>                    +#+  +:+       +#+
 */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/04 14:52:59 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <map>

#include "Client.hpp"

// DEBUGING
#include <iostream>

struct SpecifiedClient {
	Client *client;
	bool	isOp;
};

class Channel {
   public:
	Channel(std::string name, Client *const client);
	~Channel();
	void								   addNewUser(Client *const client);
	void								   removeUser(Client *const client);
	const std::string					   getUserList();
	const std::string					  &getName() const;
	std::map<std::string, SpecifiedClient> userMap;
	std::string							   topic;

   private:
	std::string _name;
};
