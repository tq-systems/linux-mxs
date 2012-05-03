/*
 * Copyright (C) 2011 IEQualize GmbH
 *   Michael Heimpold <mhe@iequalize.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#ifndef _ARCH_ARM_MACH_EXPORT_GPIO_H
#define _ARCH_ARM_MACH_EXPORT_GPIO_H

#include <linux/gpio.h>

struct export_gpio {
	unsigned      gpio;
	unsigned long flags;
	const char    *label;
	bool          direction_may_change;
	int           active_low;
};

extern int export_gpios(struct export_gpio *array, size_t num);

#endif /* _ARCH_ARM_MACH_EXPORT_GPIO_H */
