/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/11 13:47:47 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

static Flag flagArray[FLAG_COUNT] = {{INVITE_CHAR, INVITE_ONLY},
	{TOPIC_CHAR, TOPIC_RESTRICTION}, {USRLIMIT_CHAR, USERLIMIT},
	{KEY_CHAR, PASS_ONLY}, {OP_CHANGE_CHAR, OP_CHANGE}};

static uint8_t searchFlag(const char c) {
	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (c == flagArray[i].FlagChar)
			return flagArray[i].FlagMask;
	}
	return NO_MOD;
}

static bool isModeArgValid(const char c, const std::string &modeArg) {
	if (c == KEY_CHAR)
		return Utils::isThereInvalidChar(modeArg, INVALID_CHARSET_KEY) == false;
	else if (c == USRLIMIT_CHAR) {
		std::istringstream iss(modeArg);
		int				   newLimit;
		if (iss >> newLimit)
			return newLimit > 0 && newLimit <= CHANNEL_USERLIMIT;
	} else if (c == OP_CHANGE_CHAR) {
		return true;
	}
	return false;
}

// Public methods

Mode::Mode() : _mask(NO_MOD) {}

Mode::Mode(const uint8_t initialMask) : _mask(initialMask) {}

Mode::~Mode() {}

bool Mode::isSimpleFlag(const char cflag) {
	return (cflag == INVITE_CHAR || cflag == TOPIC_CHAR);
}

bool Mode::isArgFlag(const char cflag) {
	return (
		cflag == OP_CHANGE_CHAR || cflag == KEY_CHAR || cflag == USRLIMIT_CHAR);
}

modeStatus Mode::setMode(const t_modSetter setter, const char cflag,
	const std::vector<std::string> &cmd, const size_t argsIndex) {
	modeStatus status = {.hasChanged = false, .doesUseArg = false};

	if (isSimpleFlag(cflag)) {
		status = setSimpleMode(setter, cflag);
	} else if (isArgFlag(cflag)) {
		status = setArgMode(setter, cflag, cmd, argsIndex);
	}
	return status;
}

std::string Mode::getModeMessage() const {
	std::string modeMessage = MODE_MESSAGE_PREFIX;

	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (_mask & flagArray[i].FlagMask)
			modeMessage += flagArray[i].FlagChar;
	}
	return modeMessage;
}

bool Mode::isTopicProtected() const { return _mask & TOPIC_RESTRICTION; }

bool Mode::isKeyProtected() const { return _mask & PASS_ONLY; }

bool Mode::hasUserLimit() const { return _mask & USERLIMIT; }

bool Mode::isInviteOnly() const { return _mask & INVITE_ONLY; }

// Private methods

modeStatus Mode::setSimpleMode(const t_modSetter setter, const char c) {
	const uint8_t flag = searchFlag(c);
	const uint8_t oldMask = _mask;
	modeStatus	  status = {.hasChanged = false, .doesUseArg = false};

	if (flag)
		setFlags(flag, setter);
	status.hasChanged |= (oldMask != _mask);
	return status;
}

modeStatus Mode::setArgMode(const t_modSetter setter, const char c,
	const std::vector<std::string> &modeArg, const size_t modeArgIndex) {
	const uint8_t flag = searchFlag(c);
	modeStatus	  status = {.hasChanged = false, .doesUseArg = false};

	if (modeArgIndex < modeArg.size() &&
		isModeArgValid(c, modeArg[modeArgIndex])) {
		status.hasChanged = true;
	}
	if (setter == RM && (flag == PASS_ONLY || flag == USERLIMIT))
		status.hasChanged = true;
	status.doesUseArg = modeArgIndex < modeArg.size();
	setFlags(flag, setter);
	return status;
}

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_mask |= flags;
	else if (setter == RM)
		_mask &= ~flags;
}
