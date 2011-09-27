/*
 * Copyright (C) 2011 IEQualize GmbH
 *   Michael Heimpold <mhe@iequalize.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include <mach/pinctrl.h>

#include "mx28_pins.h"
#include "pingroup.h"

/* Needed by devices.c */
int enable_gpmi = { 0 };

static struct pin_desc tqma28l_emmc_pins[] = {
	{
	 .name = "SSP1_DATA0",
	 .id = PINID_GPMI_D00,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA1",
	 .id = PINID_GPMI_D01,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA2",
	 .id = PINID_GPMI_D02,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA3",
	 .id = PINID_GPMI_D03,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA4",
	 .id = PINID_GPMI_D04,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA5",
	 .id = PINID_GPMI_D05,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA6",
	 .id = PINID_GPMI_D06,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_DATA7",
	 .id = PINID_GPMI_D07,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_CMD",
	 .id = PINID_GPMI_RDY1,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .pullup = 1,
	 .drive = 1,
	 .pull = 1,
	 },
	{
	 .name = "SSP1_SCK",
	 .id = PINID_GPMI_WRN,
	 .fun = PIN_FUN2,
	 .strength = PAD_12MA,
	 .voltage = PAD_3_3V,
	 .pullup = 0,
	 .drive = 2,
	 .pull = 0,
	 },
};

void __init tqma28l_pins_init(void)
{
	pr_info("Initializing eMMC pins\n");
	pingroup_init(tqma28l_emmc_pins, ARRAY_SIZE(tqma28l_emmc_pins));
}
