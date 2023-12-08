/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:16:05 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/08 00:54:47 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define NO_MOD			  0x00
#define INVITE_ONLY		  0x01
#define TOPIC_RESTRICTION 0x02
#define PASS_ONLY		  0x04
#define USERLIMIT		  0x08
#define OP_CHANGE		  0x10
#define FLAG_COUNT		  5

#define CHANNEL_USERLIMIT 100

#define INVITE_CHAR	   'i'
#define TOPIC_CHAR	   't'
#define KEY_CHAR	   'k'
#define USRLIMIT_CHAR  'l'
#define OP_CHANGE_CHAR 'o'

#define INVALID_CHARSET_KEY ",: \b\t\n\v\f\r"

typedef enum e_modSetter { ADD, RM } t_modSetter;
typedef struct s_flag {
	char	FlagChar;
	uint8_t FlagMask;
} Flag;
typedef struct s_modeStatus {
	bool hasChanged;
	bool doesUseArg;
} modeStatus;

class Mode {
   public:
	Mode();
	Mode(const uint8_t initialMask);
	~Mode();
	uint8_t		getModeMask() const;
	modeStatus	setMode(const t_modSetter setter, const char c,
		 const std::vector<std::string> &modArg, const size_t modeArgIndex);
	std::string getModeMessage() const;

   private:
	// Attributes
	uint8_t _mask;
	// Private Methods
	void setFlags(const uint8_t flags, const t_modSetter setter);
};