/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:54 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/19 12:09:10 by rbroque          ###   ########.fr       */
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
#include <unistd.h>
#include <string>
#include <iostream>
#include "server.hpp"

///////////////////////////////
///			DEFINES			///
///////////////////////////////

# define PORT				8080
# define WELCOME_MESSAGE	"Hello from the Server"
# define MAX_CLIENT_COUNT	3
# define EXPECTED_ARG_COUNT	3
