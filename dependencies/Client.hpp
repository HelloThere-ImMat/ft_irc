/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:32:02 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/27 16:07:00 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Client {
   public:
	Client();
	explicit Client(const int sockfd);
	~Client();
	int getSocketFd() const;
	const std::string &getBuffer() const;
	void setBuffer(const std::string &incompleteMessage);
	void setSocketFd(const int socket);

   private:
	int _sockfd;
	std::string _buffer;
};
