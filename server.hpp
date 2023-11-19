/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:22 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 01:19:44 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////////////////////////////
///			INCLUDES		///
///////////////////////////////

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>

///////////////////////////////
///			DEFINES			///
///////////////////////////////

# define PORT				8080
# define WELCOME_MESSAGE	"Hello from the Server"
# define MAX_CLIENT_COUNT	3
# define EXPECTED_ARG_COUNT	3

class Data
{
	public :
		int					sockfd;
		int 				servfd;
		struct sockaddr_in	address;
		socklen_t			addrlen;
};
