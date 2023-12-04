/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/04 11:29:00 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Client.hpp"

// DEBUGING
#include <iostream>

class Channel {
	public :
		Channel(std::string name, Client *const client);
		~Channel();
		void addNewUser(Client *const client);
		void removeUser(Client *const client);
		const std::string &getName() const;
		std::vector<int>						userFds;
		std::vector<std::string>				userNicks;
		std::string								topic;
	private:
		std::string								_name;
		int										_operatorFd;
};
