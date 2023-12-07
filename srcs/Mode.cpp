/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbroque <rbroque@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 09:18:36 by rbroque           #+#    #+#             */
/*   Updated: 2023/12/07 09:35:08 by rbroque          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode() : _modeMask(NO_MOD) {}

Mode::Mode(const uint8_t initialMask) : _modeMask(initialMask) {}

Mode::~Mode() {}

void Mode::setFlags(const uint8_t flags, const t_modSetter setter) {
	if (setter == ADD)
		_modeMask |= flags;
	else if (setter == RM)
		_modeMask &= ~flags;
}

bool Mode::setMode(
	const t_modSetter setter, const char c, std::vector<std::string> &modArgs) {
	// if (c == 'k' || c == 'o' || c == 'l')
	// {
	// 	if (c == 'l')

	// 	modArgs.erase(0, 1);
	(void)modArgs;
	if (c == 'i' || c == 't') {
		const uint8_t flags = (c == 'i') ? INVITE_ONLY : TOPIC_RESTRICTION;
		setFlags(flags, setter);
		return true;
	}
	return false;
}

uint8_t Mode::getModeMask() const { return _modeMask; }