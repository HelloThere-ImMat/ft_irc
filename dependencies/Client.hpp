/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:32:02 by rbroque           #+#    #+#             */
/*   Updated: 2023/11/20 18:22:38 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Client
{
	public:
		Client();
		Client(const int sockfd);
		~Client();
		int		getSocketFd() const;
		void	setSocketFd(const int socket);
	private:
		int	_sockfd;
};
