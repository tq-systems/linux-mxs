/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <mach/pinctrl.h>

#include "mx28_pins.h"

/* Needed by devices.c */
int enable_gpmi = { 1 };

static struct pin_desc qx28_fixed_pins[] = {
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
#ifdef CONFIG_MXS_AUART0_DEVICE_ENABLE
	{
	 .name  = "AUART0.RX",
	 .id    = PINID_AUART0_RX,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART0.TX",
	 .id    = PINID_AUART0_TX,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART0.CTS",
	 .id    = PINID_AUART0_CTS,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART0.RTS",
	 .id    = PINID_AUART0_RTS,
	 .fun   = PIN_FUN1,
	 },
#endif
#ifdef CONFIG_MXS_AUART1_DEVICE_ENABLE
	{
	 .name  = "AUART1.RX",
	 .id    = PINID_AUART1_RX,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART1.TX",
	 .id    = PINID_AUART1_TX,
	 .fun   = PIN_FUN1,
	 },
#endif
#ifdef CONFIG_MXS_AUART3_DEVICE_ENABLE
	{
	 .name  = "AUART3.RX",
	 .id    = PINID_AUART3_RX,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART3.TX",
	 .id    = PINID_AUART3_TX,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART3.CTS",
	 .id    = PINID_AUART3_CTS,
	 .fun   = PIN_FUN1,
	 },
	{
	 .name  = "AUART3.RTS",
	 .id    = PINID_AUART3_RTS,
	 .fun   = PIN_FUN1,
	 },
#endif
#ifdef CONFIG_MXS_AUART4_DEVICE_ENABLE
	{
	 .name  = "AUART4.RX",
	 .id    = PINID_SSP3_MOSI,
	 .fun   = PIN_FUN2,
	 },
	{
	 .name  = "AUART4.TX",
	 .id    = PINID_SSP3_SCK,
	 .fun   = PIN_FUN2,
	 },
	{
	 .name  = "AUART4.CTS",
	 .id    = PINID_SSP3_SS0,
	 .fun   = PIN_FUN2,
	 },
	{
	 .name  = "AUART4.RTS",
	 .id    = PINID_SSP3_MISO,
	 .fun   = PIN_FUN2,
	 },
#endif
	 {
	 .name = "usb0",
	 .id = PINID_AUART2_TX, /* Power enable pin*/
	 .fun = PIN_GPIO,
	 .data = 0,
	 .output = 1,
	 },
	 {
	 .name  = "usb1",
	 .id    = PINID_AUART2_RX,
	 .fun   = PIN_GPIO,
	 .data  = 1,
	 .output = 1,
	 },

#if defined(CONFIG_USB_OTG)
	 {
	 .name 	= "usb0_id",
	 .id 	= PINID_AUART1_RTS,
	 .fun	= PIN_FUN2,
	 .data 	= 1,
	 .pull 	= 1,
	 .pullup = 1,
	 },
#endif

#if defined(CONFIG_I2C_MXS) || \
	defined(CONFIG_I2C_MXS_MODULE)
	{
	 .name = "I2C0_SCL",
	 .id = PINID_I2C0_SCL,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "I2C0_SDA",
	 .id = PINID_I2C0_SDA,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
#endif

#if defined(CONFIG_LEDS_MXS) || defined(CONFIG_LEDS_MXS_MODULE)
	{
	 .name = "LEDS_PWM0",
	 .id = PINID_AUART1_RX,
	 .fun           = PIN_FUN3,
	 .strength      = PAD_8MA,
	 .voltage       = PAD_3_3V,
	 .pullup        = 1,
	 .drive         = 1,
	 .pull          = 1,
	 },
	{
	 .name = "LEDS_PWM1",
	 .id = PINID_AUART1_TX,
	 .fun           = PIN_FUN3,
	 .strength      = PAD_8MA,
	 .voltage       = PAD_3_3V,
	 .pullup        = 1,
	 .drive         = 1,
	 .pull          = 1,
	 },
#endif
};

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE)\
	|| defined(CONFIG_FEC_L2SWITCH)
static struct pin_desc qx28_eth_pins[] = {
	{
	 .name = "ENET0_MDC",
	 .id = PINID_ENET0_MDC,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_MDIO",
	 .id = PINID_ENET0_MDIO,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_RX_EN",
	 .id = PINID_ENET0_RX_EN,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_RXD0",
	 .id = PINID_ENET0_RXD0,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_RXD1",
	 .id = PINID_ENET0_RXD1,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_TX_EN",
	 .id = PINID_ENET0_TX_EN,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_TXD0",
	 .id = PINID_ENET0_TXD0,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET0_TXD1",
	 .id = PINID_ENET0_TXD1,
	 .fun = PIN_FUN1,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive	= 1,
	 },
	{
	 .name = "ENET1_RX_EN",
	 .id = PINID_ENET0_CRS,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive = 1,
	 },
	{
	 .name = "ENET1_RXD0",
	 .id = PINID_ENET0_RXD2,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive = 1,
	 },
	{
	 .name = "ENET1_RXD1",
	 .id = PINID_ENET0_RXD3,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive = 1,
	 },
	{
	 .name = "ENET1_TX_EN",
	 .id = PINID_ENET0_COL,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive = 1,
	 },
	{
	 .name = "ENET1_TXD0",
	 .id = PINID_ENET0_TXD2,
	 .fun = PIN_FUN2,
	 .strength = PAD_8MA,
	 .pull = 1,
	 .pullup = 1,
	 .voltage = PAD_3_3V,
	 .drive = 1,
	 },
	{
	 .name = "ENET1_TXD1",
	 .id = PINID_ENET0_TXD3,
	 .fun = PIN_FUN2,
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
	 .drive	= 1,
	 },
};
#endif

static struct pin_desc qx28_gpmi_pins[] = {
	{
	 .name     = "GPMI D0",
	 .id       = PINID_GPMI_D00,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	.name     = "GPMI D1",
	.id       = PINID_GPMI_D01,
	.fun      = PIN_FUN1,
	.strength = PAD_4MA,
	.voltage  = PAD_1_8V,
	.pullup   = 0,
	.drive    = !0
	 },
	{
	 .name     = "GPMI D2",
	 .id       = PINID_GPMI_D02,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI D3",
	 .id       = PINID_GPMI_D03,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI D4",
	 .id       = PINID_GPMI_D04,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI D5",
	 .id       = PINID_GPMI_D05,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI D6",
	 .id       = PINID_GPMI_D06,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI D7",
	 .id       = PINID_GPMI_D07,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI CE0-",
	 .id       = PINID_GPMI_CE0N,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI RDY0",
	 .id       = PINID_GPMI_RDY0,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI RD-",
	 .id       = PINID_GPMI_RDN,
	 .fun      = PIN_FUN1,
	 .strength = PAD_12MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI WR-",
	 .id       = PINID_GPMI_WRN,
	 .fun      = PIN_FUN1,
	 .strength = PAD_12MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI ALE",
	 .id       = PINID_GPMI_ALE,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
	{
	 .name     = "GPMI CLE",
	 .id       = PINID_GPMI_CLE,
	 .fun      = PIN_FUN1,
	 .strength = PAD_4MA,
	 .voltage  = PAD_1_8V,
	 .pullup   = 0,
	 .drive    = !0
	 },
};

#if defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE)
static struct pin_desc qx28_spi_pins[] = {
	{
	 .name	= "SSP2 MOSI",
	 .id	= PINID_SSP2_MOSI,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_4MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 },
	{
	 .name	= "SSP2 MISO",
	 .id	= PINID_SSP2_MISO,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_4MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 },
	{
	 .name	= "SSP2 SCK",
	 .id	= PINID_SSP2_SCK,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_4MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 },
	{
	 .name	= "SSP2 SS0",
	 .id	= PINID_SSP2_SS0,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 },
};
#endif

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE)\
	|| defined(CONFIG_FEC_L2SWITCH)
int qx28_enet_gpio_init(void)
{
	/* reset phy */
	gpio_request(MXS_PIN_TO_GPIO(PINID_SSP0_DATA6), "PHY_RESET");
	gpio_direction_output(MXS_PIN_TO_GPIO(PINID_SSP0_DATA6), 0);

	/*
	 * Before timer bug fix(set wrong match value of timer),
	 * mdelay(10) delay 50ms actually.
	 * So change delay to 50ms after timer issue fix.
	 */
	mdelay(50);
	gpio_direction_output(MXS_PIN_TO_GPIO(PINID_SSP0_DATA6), 1);

	return 0;
}
#else
int qx28_enet_gpio_init(void)
{
	return 0;
}
#endif

void qx28_enet_io_lowerpower_enter(void)
{
}

void qx28_enet_io_lowerpower_exit(void)
{
}

void __init qx28_init_pin_group(struct pin_desc *pins, unsigned count)
{
	int i;
	struct pin_desc *pin;
	for (i = 0; i < count; i++) {
		pin = pins + i;
		if (pin->fun == PIN_GPIO)
			gpio_request(MXS_PIN_TO_GPIO(pin->id), pin->name);
		else
			mxs_request_pin(pin->id, pin->fun, pin->name);
		if (pin->drive) {
			mxs_set_strength(pin->id, pin->strength, pin->name);
			mxs_set_voltage(pin->id, pin->voltage, pin->name);
		}
		if (pin->pull)
			mxs_set_pullup(pin->id, pin->pullup, pin->name);
		if (pin->fun == PIN_GPIO) {
			if (pin->output)
				gpio_direction_output(MXS_PIN_TO_GPIO(pin->id),
							pin->data);
			else
				gpio_direction_input(MXS_PIN_TO_GPIO(pin->id));
		}
	}
}

void __init qx28_pins_init(void)
{
	qx28_init_pin_group(qx28_fixed_pins, ARRAY_SIZE(qx28_fixed_pins));

	pr_info("Initializing GPMI pins\n");
	qx28_init_pin_group(qx28_gpmi_pins, ARRAY_SIZE(qx28_gpmi_pins));

#if defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE)
	qx28_init_pin_group(qx28_spi_pins, ARRAY_SIZE(qx28_spi_pins));
#endif

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE) || defined(CONFIG_FEC_L2SWITCH)
	qx28_init_pin_group(qx28_eth_pins, ARRAY_SIZE(qx28_eth_pins));
#endif
}
