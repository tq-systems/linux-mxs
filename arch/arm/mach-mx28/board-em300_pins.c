/*
 * Copyright (C) 2011-2012 IEQualize GmbH
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <mach/pinctrl.h>

#include "mx28_pins.h"
#include "pingroup.h"

static struct pin_desc em300_fixed_pins[] = {
	/* Debug UART */
	{
		.name = "DUART.RX",
		.id = PINID_PWM0,
		.fun = PIN_FUN3,
	},
	{
		.name = "DUART.TX",
		.id = PINID_PWM1,
		.fun = PIN_FUN3,
	},
	/* RS485 */
	{
		.name = "AUART1.RX",
		.id = PINID_AUART1_RX,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART1.TX",
		.id = PINID_AUART1_TX,
		.fun = PIN_FUN1,
	},
	/* USB power enable pins */
	{
		.name = "USB0.ENABLE",
		.id = PINID_SAIF1_SDATA0,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
	},
	/* USB overcurrent pins */
	{
		.name = "USB0.OVERCURRENT",
		.id = PINID_SSP2_SS2,
		.fun = PIN_FUN3,
	},
	/* USB0 ID */
	{
		.name = "USB0.ID",
		.id = PINID_PWM2,
		.fun = PIN_FUN2,
		.data = 1,
		.pull = 1,
		.pullup = 1,
	},
	/* I2C */
	{
		.name = "I2C0.SCL",
		.id = PINID_I2C0_SCL,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "I2C0.SDA",
		.id = PINID_I2C0_SDA,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	/* Ethernet */
	{
		.name = "ENET0.MDC",
		.id = PINID_ENET0_MDC,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.MDIO",
		.id = PINID_ENET0_MDIO,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.RX_EN",
		.id = PINID_ENET0_RX_EN,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.RXD0",
		.id = PINID_ENET0_RXD0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.RXD1",
		.id = PINID_ENET0_RXD1,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive	 = 1,
	},
	{
		.name = "ENET0.TX_EN",
		.id = PINID_ENET0_TX_EN,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.TXD0",
		.id = PINID_ENET0_TXD0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0.TXD1",
		.id = PINID_ENET0_TXD1,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET_CLK",
		.id = PINID_ENET_CLK,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
};

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE) || defined(CONFIG_FEC_L2SWITCH)
int em300_enet_gpio_init(void)
{
	int phy_reset_gpio = MXS_PIN_TO_GPIO(PINID_GPMI_CE1N);

	/* reset phy */
	gpio_request(phy_reset_gpio, "ETH0.PHY_RESET");
	gpio_direction_output(phy_reset_gpio, 0);
	mdelay(50);
	gpio_direction_output(phy_reset_gpio, 1);

	return 0;
}
#else
int em300_enet_gpio_init(void)
{
	return 0;
}
#endif

void em300_enet_io_lowerpower_enter(void)
{
}

void em300_enet_io_lowerpower_exit(void)
{
}

void __init em300_pins_init(void)
{
	pingroup_init(em300_fixed_pins, ARRAY_SIZE(em300_fixed_pins));
}
