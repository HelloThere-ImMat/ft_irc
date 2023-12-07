/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:16:05 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/07 09:47:18 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#define NO_MOD			  0x00
#define INVITE_ONLY		  0x01
#define TOPIC_RESTRICTION 0x02
#define PASS_ONLY		  0x04
#define USERLIMIT		  0x08

typedef enum e_modSetter { ADD, RM } t_modSetter;

class Mode {
   public:
	Mode();
	Mode(const uint8_t initialMask);
	~Mode();
	uint8_t		getModeMask() const;
	bool		setMode(const t_modSetter setter, const char c,
			   std::vector<std::string> &modArgs);
	std::string getModeMessage() const;

   private:
	// Attributes
	uint8_t _mask;
	// Private Methods
	void setFlags(const uint8_t flags, const t_modSetter setter);
};