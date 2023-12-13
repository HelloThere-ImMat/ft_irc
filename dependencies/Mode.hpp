/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:16:05 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/12 17:25:00 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>

#include "Utils.hpp"

#define NO_MOD			  0x00
#define INVITE_ONLY		  0x01
#define TOPIC_RESTRICTION 0x02
#define PASS_ONLY		  0x04
#define USERLIMIT		  0x08
#define OP_CHANGE		  0x10
#define FLAG_COUNT		  5

#define CHANNEL_USERLIMIT 100

#define INVITE_CHAR			'i'
#define TOPIC_CHAR			't'
#define KEY_CHAR			'k'
#define USRLIMIT_CHAR		'l'
#define OP_CHANGE_CHAR		'o'
#define MODE_MESSAGE_PREFIX "+"

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
typedef struct s_modeConfig {
	uint8_t		mask;
	uint		userlimit;
	std::string password;
} modeConfig;

class Mode {
   public:
	Mode();
	Mode(const uint8_t initialMask);
	~Mode();
	// Setters
	void	   setUserlimit(const uint userlimit);
	void	   setPassword(const std::string &password);
	modeStatus setMode(const t_modSetter setter, const char cflag,
		const std::vector<std::string> &cmd, const size_t argsIndex);
	// Getters
	uint		getUserLimit() const;
	std::string getPassword() const;
	bool		isKeyProtected() const;
	bool		isTopicProtected() const;
	bool		hasUserLimit() const;
	bool		isInviteOnly() const;
	bool		isSimpleFlag(const char cflag) const;
	bool		isArgFlag(const char cflag) const;
	std::string getModeMessage() const;

   private:
	// Attributes
	modeConfig _config;
	// uint8_t		_mask;
	// uint		_userlimit;
	// std::string _password;
	// Private Methods
	modeStatus setSimpleMode(const t_modSetter setter, const char c);
	modeStatus setArgMode(const t_modSetter setter, const char c,
		const std::vector<std::string> &modeArg, const size_t modeArgIndex);
	void	   setFlags(const uint8_t flags, const t_modSetter setter);
};