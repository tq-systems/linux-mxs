/*
 * Copyright (C) 2011 IEQualize GmbH
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
		.name = "AUART0.RX",
		.id = PINID_AUART0_RX,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART0.TX",
		.id = PINID_AUART0_TX,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART0.CTS",
		.id = PINID_AUART0_CTS,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART0.RTS",
		.id = PINID_AUART0_RTS,
		.fun = PIN_FUN1,
	},
#endif
#ifdef CONFIG_MXS_AUART1_DEVICE_ENABLE
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
#endif
#ifdef CONFIG_MXS_AUART2_DEVICE_ENABLE
	{
		.name = "AUART2.RX",
		.id = PINID_AUART2_RX,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART2.TX",
		.id = PINID_AUART2_TX,
		.fun = PIN_FUN1,
	},
#endif
#ifdef CONFIG_MXS_AUART3_DEVICE_ENABLE
	{
		.name = "AUART3.RX",
		.id = PINID_SSP2_MISO,
		.fun = PIN_FUN2,
	},
	{
		.name = "AUART3.TX",
		.id = PINID_SSP2_SS0,
		.fun = PIN_FUN2,
	},
	{
		.name = "AUART3.CTS",
		.id = PINID_AUART3_CTS,
		.fun = PIN_FUN1,
	},
	{
		.name = "AUART3.RTS",
		.id = PINID_AUART3_RTS,
		.fun = PIN_FUN1,
	},
#endif
#ifdef CONFIG_MXS_AUART4_DEVICE_ENABLE
	{
		.name = "AUART4.RX",
		.id = PINID_SAIF0_BITCLK,
		.fun = PIN_FUN3,
	},
	{
		.name = "AUART4.TX",
		.id = PINID_SAIF0_SDATA0,
		.fun = PIN_FUN3,
	},
	{
		.name = "AUART4.CTS",
		.id = PINID_SAIF0_MCLK,
		.fun = PIN_FUN3,
	},
	{
		.name = "AUART4.RTS",
		.id = PINID_SAIF0_LRCLK,
		.fun = PIN_FUN3,
	},
#endif

#if defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE)
	{
		.name = "SSP2 MOSI",
#if !defined(CONFIG_MXS_AUART3_DEVICE_ENABLE) && !defined(CONFIG_MXS_AUART2_DEVICE_ENABLE)
		.id = PINID_SSP2_MOSI,
		.fun = PIN_FUN1,
#else
		.id = PINID_SSP0_DATA6,
		.fun = PIN_FUN2,
#endif
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 MISO",
#if !defined(CONFIG_MXS_AUART3_DEVICE_ENABLE) && !defined(CONFIG_MXS_AUART2_DEVICE_ENABLE)
		.id = PINID_SSP2_MISO,
		.fun = PIN_FUN1,
#else
		.id = PINID_SSP0_DATA4,
		.fun = PIN_FUN2,
#endif
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 SCK",
#if !defined(CONFIG_MXS_AUART3_DEVICE_ENABLE) && !defined(CONFIG_MXS_AUART2_DEVICE_ENABLE)
		.id = PINID_SSP2_SCK,
		.fun = PIN_FUN1,
#else
		.id = PINI_SSP0_DATA7,
		.fun = PIN_FUN2,
#endif
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 CS0",
#if !defined(CONFIG_MXS_AUART3_DEVICE_ENABLE) && !defined(CONFIG_MXS_AUART2_DEVICE_ENABLE)
		.id = PINID_SSP2_SS0,
		.fun = PIN_FUN1,
#else
		.id = PINID_SSP0_DATA5,
		.fun = PIN_FUN2,
#endif
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
#endif

	/* USB power enable pins */
	{
		.name = "USB0_ENABLE",
		.id = PINID_SSP0_DATA5, /* Remark: change to PINID_AUART2_TX for VCxxx */
		.fun = PIN_GPIO,
		.data = 0,
		.output = 1,
	},
	{
		.name = "USB1_ENABLE",
		.id = PINID_AUART2_RX,
		.fun = PIN_GPIO,
		.data = 0,
		.output = 1,
	},
	/* USB overcurrent pins */
	{
		.name = "USB0_OVERCURRENT",
		.id = PINID_SSP2_SS2,
		.fun = PIN_FUN3,
	},
	{
		.name = "USB1_OVERCURRENT",
		.id = PINID_SSP2_SS1,
		.fun = PIN_FUN3,
	},
#if defined(CONFIG_USB_OTG)
	{
		.name = "USB0_ID",
		.id = PINID_PWM2,
		.fun = PIN_FUN2,
	},
#endif

#if defined(CONFIG_I2C_MXS) || defined(CONFIG_I2C_MXS_MODULE)
	{
		.name = "I2C0_SCL",
		.id = PINID_I2C0_SCL,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "I2C0_SDA",
		.id = PINID_I2C0_SDA,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
#endif

#if defined(CONFIG_MMC_MXS) || defined(CONFIG_MMC_MXS_MODULE)
	/* Configurations of SSP0 SD/MMC port pins */
	{
		.name = "SSP0_DATA0",
		.id = PINID_SSP0_DATA0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 1,
		.drive = 1,
		.pull = 1,
	},
	{
		.name = "SSP0_DATA1",
		.id = PINID_SSP0_DATA1,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 1,
		.drive = 1,
		.pull = 1,
	},
	{
		.name = "SSP0_DATA2",
		.id = PINID_SSP0_DATA2,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 1,
		.drive = 1,
		.pull = 1,
	},
	{
		.name = "SSP0_DATA3",
		.id = PINID_SSP0_DATA3,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 1,
		.drive = 1,
		.pull = 1,
	},
	{
		.name = "SSP0_CMD",
		.id = PINID_SSP0_CMD,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 1,
		.drive = 1,
		.pull = 1,
	},
	{
		.name = "SSP0_DETECT",
		.id = PINID_SSP0_DETECT,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.pullup = 0,
		.drive = 1,
		.pull = 0,
	},
	{
		.name = "SSP0_SCK",
		.id = PINID_SSP0_SCK,
		.fun = PIN_FUN1,
		.strength = PAD_12MA,
		.voltage = PAD_3_3V,
		.pullup = 0,
		.drive = 1,
		.pull = 0,
	},
	/* SSP0_WP is requested via GPIO in device.c */
#endif

#if defined(CONFIG_CAN_FLEXCAN) || defined(CONFIG_CAN_FLEXCAN_MODULE)
	{
		.name = "CAN0_TX",
		.id = PINID_GPMI_RDY2,
		.fun = PIN_FUN2,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.pullup = 0,
		.drive = 1,
		.pull = 0,
	},
	{
		.name = "CAN0_RX",
		.id = PINID_GPMI_RDY3,
		.fun = PIN_FUN2,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.pullup = 0,
		.drive = 1,
		.pull = 0,
	},
#endif
};

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE) || defined(CONFIG_FEC_L2SWITCH)
static struct pin_desc qx28_eth_pins[] = {
	{
		.name = "ENET0_MDC",
		.id = PINID_ENET0_MDC,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_MDIO",
		.id = PINID_ENET0_MDIO,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_RX_EN",
		.id = PINID_ENET0_RX_EN,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_RXD0",
		.id = PINID_ENET0_RXD0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_RXD1",
		.id = PINID_ENET0_RXD1,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive	 = 1,
	},
	{
		.name = "ENET0_TX_EN",
		.id = PINID_ENET0_TX_EN,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_TXD0",
		.id = PINID_ENET0_TXD0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "ENET0_TXD1",
		.id = PINID_ENET0_TXD1,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.pull = 1,
		.pullup = 1,
		.voltage = PAD_3_3V,
		.drive = 1,
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
		.drive = 1,
	},
};
#endif

#if defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE)
static struct pin_desc qx28_spi_pins[] = {
	{
		.name = "SSP2 MOSI",
		.id = PINID_SSP2_MOSI,
		.fun = PIN_FUN1,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 MISO",
		.id = PINID_SSP2_MISO,
		.fun = PIN_FUN1,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 SCK",
		.id = PINID_SSP2_SCK,
		.fun = PIN_FUN1,
		.strength = PAD_4MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
	{
		.name = "SSP2 SS0",
		.id = PINID_SSP2_SS0,
		.fun = PIN_FUN1,
		.strength = PAD_8MA,
		.voltage = PAD_3_3V,
		.drive = 1,
	},
};
#endif

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE) || defined(CONFIG_FEC_L2SWITCH)
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

void __init qx28_pins_init(void)
{
	pingroup_init(qx28_fixed_pins, ARRAY_SIZE(qx28_fixed_pins));

#if defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE)
	pingroup_init(qx28_spi_pins, ARRAY_SIZE(qx28_spi_pins));
#endif

#if defined(CONFIG_FEC) || defined(CONFIG_FEC_MODULE) || defined(CONFIG_FEC_L2SWITCH)
	pingroup_init(qx28_eth_pins, ARRAY_SIZE(qx28_eth_pins));
#endif
}
