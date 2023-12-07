/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/07 16:15:43 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

static Flag flagArray[FLAG_COUNT] = {{'i', INVITE_ONLY},
	{'t', TOPIC_RESTRICTION}, {'l', USERLIMIT}, {'k', PASS_ONLY},
	{'o', OP_CHANGE}};

static uint8_t searchFlags(const char c) {
	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (c == flagArray[i].FlagChar)
			return flagArray[i].FlagMask;
	}
	return NO_MOD;
}

// Public methods

Mode::Mode() : _mask(NO_MOD) {}

Mode::Mode(const uint8_t initialMask) : _mask(initialMask) {}

Mode::~Mode() {}

bool Mode::setMode(const t_modSetter setter, const char c,
	std::vector<std::string> &modeArgs) {
	uint8_t flags;

	if (c == 'k' || c == 'o' || c == 'l') {
		if (setter == ADD && modeArgs.empty() == false) {
			modeArgs.erase(modeArgs.begin());
		}
		if (setter == RM || modeArgs.empty() == false)
			flags = searchFlags(c);
	} else
		flags = searchFlags(c);
	if (flags)
		setFlags(flags, setter);
	return flags != NO_MOD;
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
