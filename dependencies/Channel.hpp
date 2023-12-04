/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:37:48 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/04 14:38:26 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <map>

#include "Client.hpp"

// DEBUGING
#include <iostream>

typedef struct s_specifiedClient {
	Client *client;
	bool	isOp;
} SpecifiedClient;

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
