/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/07 09:59:44 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

// Public methods

Mode::Mode() : _mask(NO_MOD) {}

Mode::Mode(const uint8_t initialMask) : _mask(initialMask) {}

Mode::~Mode() {}

bool Mode::setMode(const t_modSetter setter, const char c,
	std::vector<std::string> &modeArgs) {
	uint8_t flags = NO_MOD;
	if (c == 'k' || c == 'o' || c == 'l') {
		if (c == 'l' && modeArgs.empty() == false) {
			flags = USERLIMIT;
			modeArgs.erase(modeArgs.begin());
		}
	} else if (c == 'i' || c == 't') {
		flags = (c == 'i') ? INVITE_ONLY : TOPIC_RESTRICTION;
	}
	if (flags)
		setFlags(flags, setter);
	return flags != NO_MOD;
}

uint8_t Mode::getModeMask() const { return _mask; }

std::string Mode::getModeMessage() const {
	std::string modeMessage = "+";

	if (_mask & INVITE_ONLY)
		modeMessage += "i";
	if (_mask & USERLIMIT)
		modeMessage += "l";
	if (_mask & TOPIC_RESTRICTION)
		modeMessage += "t";
	if (_mask & PASS_ONLY)
		modeMessage += "k";
	return modeMessage;
}

// Private methods

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_mask |= flags;
	else if (setter == RM)
		_mask &= ~flags;
}
