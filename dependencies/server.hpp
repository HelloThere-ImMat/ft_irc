/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 12:10:57 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
#include <sys/socket.h>

class Data
{
	public :
		int					sockfd;
		int 				servfd;
		struct sockaddr_in	address;
		socklen_t			addrlen;
};

int	setUpServer(Data *const obj);
