/*
 * Copyright (C) 2012 IEQualize GmbH
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

static struct pin_desc homebox_fixed_pins[] = {
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
		.name = "AUART0.RX",
		.id = PINID_AUART0_RX,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART0.TX",
		.id = PINID_AUART0_TX,
		.fun = PIN_FUN1,
	},
	/* SPI */
	{
		.name = "SSP2.MOSI",
		.id = PINID_SSP0_DATA6,
		.fun = PIN_FUN2,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2.MISO",
		.id = PINID_SSP0_DATA4,
		.fun = PIN_FUN2,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2.SCK",
		.id = PINID_SSP0_DATA7,
		.fun = PIN_FUN2,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2.CS0",
		.id = PINID_SSP0_DATA5,
		.fun = PIN_FUN2,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	/* VCC 3V3 disable pin */
	{
		.name = "VCC3V3.DISABLEN",
		.id = PINID_PWM4,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
	},
	/* CMS disable pin */
	{
		.name = "CMS.DISABLEN",
		.id = PINID_SAIF1_SDATA0,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
	},
	/* IMEAS disable pin */
	{
		.name = "IMEAS.DISABLEN",
		.id = PINID_SSP2_SS2,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
	},
	/* WLAN disable pin */
	{
		.name = "WLAN.DISABLEN",
		.id = PINID_GPMI_CLE,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
	},
	/* USB power enable pin */
	{
		.name = "USB0.ENABLE",
		.id = PINID_PWM3,
		.fun = PIN_GPIO,
		.data = 1,
		.output = 1,
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
int homebox_enet_gpio_init(void)
{
	int phy_reset_gpio = MXS_PIN_TO_GPIO(PINID_LCD_D00);

	/* reset phy */
	gpio_request(phy_reset_gpio, "ETH0.PHY_RESET");
	gpio_direction_output(phy_reset_gpio, 0);
	mdelay(50);
	gpio_direction_output(phy_reset_gpio, 1);

	return 0;
}
#else
int homebox_enet_gpio_init(void)
{
	return 0;
}
#endif

void homebox_enet_io_lowerpower_enter(void)
{
}

void homebox_enet_io_lowerpower_exit(void)
{
}

void __init homebox_pins_init(void)
{
	pingroup_init(homebox_fixed_pins, ARRAY_SIZE(homebox_fixed_pins));
}
