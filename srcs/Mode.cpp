/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/07 22:43:51 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

static Flag flagArray[FLAG_COUNT] = {{INVITE_CHAR, INVITE_ONLY},
	{TOPIC_CHAR, TOPIC_RESTRICTION}, {USRLIMIT_CHAR, USERLIMIT},
	{KEY_CHAR, PASS_ONLY}, {OP_CHANGE_CHAR, OP_CHANGE}};

static uint8_t searchFlags(const char c) {
	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (c == flagArray[i].FlagChar)
			return flagArray[i].FlagMask;
	}
	return NO_MOD;
}

static bool isThereInvalidChar(
	const std::string &str, const std::string &invalidCharset) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (invalidCharset.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

static bool isModeArgValid(const char c, const std::string &modeArg) {
	if (c == KEY_CHAR)
		return isThereInvalidChar(modeArg, INVALID_CHARSET_KEY) == false;
	return false;
}

// Public methods

Mode::Mode() : _mask(NO_MOD) {}

Mode::Mode(const uint8_t initialMask) : _mask(initialMask) {}

Mode::~Mode() {}

modeStatus Mode::setMode(const t_modSetter setter, const char c,
	std::vector<std::string> &modeArgs) {
	const uint8_t oldMask = _mask;
	uint8_t		  flags = NO_MOD;
	modeStatus	  status = {.hasChanged = false, .doesUseArg = false};

	if (c == KEY_CHAR || c == OP_CHANGE_CHAR || c == USRLIMIT_CHAR) {
		if (setter == ADD && modeArgs.empty() == false) {
			if (isModeArgValid(c, modeArgs[0])) {
				flags = searchFlags(c);
				status.hasChanged = true;
			}
			status.doesUseArg = true;
		} else if (setter == RM)
			flags = searchFlags(c);
	} else
		flags = searchFlags(c);
	if (flags)
		setFlags(flags, setter);
	status.hasChanged |= (oldMask != _mask);
	return status;
}

uint8_t Mode::getModeMask() const { return _mask; }

std::string Mode::getModeMessage() const {
	std::string modeMessage = "+";

	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (_mask & flagArray[i].FlagMask)
			modeMessage += flagArray[i].FlagChar;
	}
	return modeMessage;
}

// Private methods

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_mask |= flags;
	else if (setter == RM)
		_mask &= ~flags;
}
