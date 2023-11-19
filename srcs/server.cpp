/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:10:42 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 12:10:44 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	setUpServer(Data *const obj)
{
	int opt = 1;

	//Setting address to local host
	obj->address.sin_family = AF_INET;
	obj->address.sin_addr.s_addr = INADDR_ANY;
	obj->address.sin_port = htons(PORT);

	try {
		if ((obj->servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw("creating socket");
		if (setsockopt(obj->servfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			throw("setting sock options");
		if (bind(obj->servfd, (struct sockaddr*)&obj->address, sizeof(obj->address)) < 0)
			throw("binding to address");
	}
	catch(std::string &e)
	{
		std::cout << "Error : Setup : " << e << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
