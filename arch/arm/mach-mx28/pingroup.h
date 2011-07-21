/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2011 IEQualize GmbH
 *   Michael Heimpold <mhe@iequalize.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#ifndef _ARCH_ARM_MACH_PINGROUP_H
#define _ARCH_ARM_MACH_PINGROUP_H

#include <linux/kernel.h>
#include <linux/init.h>

#include <mach/pinctrl.h>

extern void pingroup_init(struct pin_desc *pins, unsigned count);

#endif /* _ARCH_ARM_MACH_PINGROUP_H */
