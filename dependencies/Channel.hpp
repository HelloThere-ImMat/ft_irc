/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:53:30 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 16:03:45 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Client.hpp"

// DEBUGING
#include <iostream>

class Channel {
	public :
		Channel(std::string name, int operatorFd);
		Channel() {}
		void addNewUser(int userFd);
		void removeUser(int userFd);
		std::vector<int>						_userFds;
	private:
		std::string								_name;
		int										_operatorFd;
};
