/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/05 11:25:17 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "SendCmd.hpp"
#include <map>

#define OP_PREFIX	"@"

struct SpecifiedClient
{
	const Client *client;
	bool 	isOp;
};

class Channel {
	public :
		Channel(const std::string name, const Client *const client);
		~Channel();
		void addNewUser(const Client *const client);
		void removeUser(const Client *const client);
		const std::string getUserList() const;
		const std::string &getName() const;
		void sendToChannel(const Client *const client, std::string message, bool sendToSelf) const;
		bool userIsInChannel(const Client *const client);
	private:
		std::map<std::string, SpecifiedClient>	userMap;
		std::string								topic;
		const std::string						_name;
};
