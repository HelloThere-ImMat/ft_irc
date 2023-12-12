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

static bool isModeArgValid(const uint8_t flag, const std::string &modeArg) {
	if (flag == PASS_ONLY)
		return Utils::isThereInvalidChar(modeArg, INVALID_CHARSET_KEY) == false;
	else if (flag == USERLIMIT) {
		std::istringstream iss(modeArg);
		int				   newLimit;
		if (iss >> newLimit)
			return newLimit > 0 && newLimit <= CHANNEL_USERLIMIT;
	} else if (flag == OP_CHANGE) {
		return true;
	}
	return false;
}

static bool canArgModeBeSet(const t_modSetter setter, const uint8_t flag,
	const std::vector<std::string> &modeArg, const size_t modeArgIndex) {
	return setter == RM ||
		   (modeArgIndex < modeArg.size() &&
			   (flag == OP_CHANGE ||
				   (setter == ADD &&
					   isModeArgValid(flag, modeArg[modeArgIndex]))));
}

// Public methods

Mode::Mode() {
	_config.mask = NO_MOD;
	_config.userlimit = 0;
	_config.password = "";
}

Mode::Mode(const uint8_t initialMask) {
	_config.mask = initialMask;
	_config.userlimit = 0;
	_config.password = "";
}

Mode::~Mode() {}

// Setters

void Mode::setPassword(const std::string &password) {
	_config.password = password;
}

void Mode::setUserlimit(const uint userlimit) { _config.userlimit = userlimit; }

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

uint Mode::getUserLimit() const { return _config.userlimit; }

std::string Mode::getPassword() const { return _config.password; }

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
		if (_config.mask & flagArray[i].FlagMask)
			modeMessage += flagArray[i].FlagChar;
	}
	return modeMessage;
}

bool Mode::isTopicProtected() const { return _config.mask & TOPIC_RESTRICTION; }

bool Mode::isKeyProtected() const { return _config.mask & PASS_ONLY; }

bool Mode::hasUserLimit() const { return _config.mask & USERLIMIT; }

bool Mode::isInviteOnly() const { return _config.mask & INVITE_ONLY; }

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

	if (canArgModeBeSet(setter, flag, modeArg, modeArgIndex)) {
		setFlags(flag, setter);
		status.hasChanged = true;
	}
	status.doesUseArg = modeArgIndex < modeArg.size();
	return status;
}

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_config.mask |= flags;
	else if (setter == RM)
		_config.mask &= ~flags;
}
