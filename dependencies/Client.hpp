/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:32:02 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/01 15:02:42 by mat              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <unistd.h>

#include <string>

#define EMPTY_LOGIN 0x00
#define CAP_LOGIN	0x01
#define PASS_LOGIN	0x02
#define NICK_LOGIN	0x04
#define USER_LOGIN	0x08
#define LOGGED		0x0f

class Client {
   public:
	Client(const Client &ref);
	explicit Client(const int sockfd);
	~Client();
	// Getters
	int				   getSocketFd() const;
	uint8_t			   getLogMask() const;
	const std::string &getNickname() const;
	const std::string &getUsername() const;
	const std::string &getLastCmd() const;
	const std::string &getLastArg() const;
	const std::string &getBuffer() const;
	// Setters
	void setBuffer(const std::string &incompleteMessage);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setLastCmd(const std::string &lastCmd);
	void setLastArg(const std::string &lastArg);
	void addToLoginMask(const uint8_t mask);
	// Members
	bool isAuthenticated() const;
	void clearBuffer();

   private:
	const int	_sockfd;
	uint8_t		_loginMask;
	std::string _nickname;
	std::string _username;
	std::string _lastCmd;
	std::string _lastArg;
	std::string _buffer;
};
