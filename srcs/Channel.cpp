/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:18:12 by mat               #+#    #+#             */
/*   Updated: 2023/12/01 17:07:39 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, int operatorFd)
{
	_name = name;
	_userFds.push_back(operatorFd);
	std::cout << "Successfully created a Channel" << std::endl;
}


void Channel::addNewUser(int userFd)
{
	std::cout << "Successfully joined a Channel" << std::cout;
	_userFds.push_back(userFd);
}

void Channel::removeUser(int userFd)
{
	for (std::vector<int>::iterator it = _userFds.begin(); it != _userFds.end(); it++)
	{
		if (*it == userFd)
			_userFds.erase(it);
	}
}
