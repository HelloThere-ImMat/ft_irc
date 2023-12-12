/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/12 13:50:54y rbroque          ###   ########.fr       */
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

Mode::Mode() : _mask(NO_MOD), _userlimit(0) {}

Mode::Mode(const uint8_t initialMask) : _mask(initialMask), _userlimit(0) {}

Mode::~Mode() {}

// Setters

void Mode::setPassword(const std::string &password) { _password = password; }

void Mode::setUserlimit(const uint userlimit) { _userlimit = userlimit; }

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

// Getters

uint Mode::getUserLimit() const { return _userlimit; }

std::string Mode::getPassword() const { return _password; }

bool Mode::isSimpleFlag(const char cflag) const {
	return (cflag == INVITE_CHAR || cflag == TOPIC_CHAR);
}

bool Mode::isArgFlag(const char cflag) const {
	return (
		cflag == OP_CHANGE_CHAR || cflag == KEY_CHAR || cflag == USRLIMIT_CHAR);
}

std::string Mode::getModeMessage() const {
	std::string modeMessage = MODE_MESSAGE_PREFIX;

	for (size_t i = 0; i < FLAG_COUNT; ++i) {
		if (_mask & flagArray[i].FlagMask)
			modeMessage += flagArray[i].FlagChar;
	}
	return modeMessage;
}

bool Mode::hasChanged() const {
	static modeConfig old = {.mask = NO_MOD, .userlimit = 0, .password = ""};
	bool hasChanged = old.mask != _mask || old.userlimit != _userlimit ||
					  old.password != _password;
	old.mask = _mask;
	old.userlimit = _userlimit;
	old.password = _password;
	return hasChanged;
}

bool Mode::isTopicProtected() const { return _mask & TOPIC_RESTRICTION; }

bool Mode::isKeyProtected() const { return _mask & PASS_ONLY; }

bool Mode::hasUserLimit() const { return _mask & USERLIMIT; }

bool Mode::isInviteOnly() const { return _mask & INVITE_ONLY; }

// Private methods

modeStatus Mode::setSimpleMode(const t_modSetter setter, const char c) {
	const uint8_t flag = searchFlag(c);
	modeStatus	  status = {.hasChanged = true, .doesUseArg = false};

	setFlags(flag, setter);
	return status;
}

modeStatus Mode::setArgMode(const t_modSetter setter, const char c,
	const std::vector<std::string> &modeArg, const size_t modeArgIndex) {
	const uint8_t flag = searchFlag(c);
	modeStatus	  status = {.hasChanged = false, .doesUseArg = false};

	if (flag == OP_CHANGE && modeArgIndex < modeArg.size()) {
		status.hasChanged = true;
		status.doesUseArg = true;
	} else if (modeArgIndex < modeArg.size()) {
		if (setter == ADD && isModeArgValid(c, modeArg[modeArgIndex])) {
			setFlags(flag, setter);
			status.hasChanged = true;
			status.doesUseArg = true;
		}
	}
	if (setter == RM) {
		setFlags(flag, setter);
		status.hasChanged = true;
		status.doesUseArg = modeArgIndex < modeArg.size();
	}
	return status;
}

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_mask |= flags;
	else if (setter == RM)
		_mask &= ~flags;
}
