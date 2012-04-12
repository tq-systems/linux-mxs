/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2012 IEQualize GmbH
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
#include <linux/io.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/device.h>
#include <mach/pinctrl.h>

#include <linux/spi/spi.h>

#include <linux/input.h>
#include <linux/gpio_buttons.h>
#include <linux/leds.h>

#include "device.h"
#include "module-tqma28l.h"
#include "mx28_pins.h"
#include "board-homebox.h"

static struct gpio_button homebox_gpio_buttons[] = {
	{
		.type = EV_KEY,
		.code = KEY_F1,
		.gpio = MXS_PIN_TO_GPIO(PINID_GPMI_CE0N),
		.desc = "Teach-In",
		.active_low = 1,
	},
	{
		.type = EV_KEY,
		.code = KEY_F9,
		.gpio = MXS_PIN_TO_GPIO(PINID_SAIF0_SDATA0),
		.desc = "CMS Power Fault",
		.active_low = 1,
	},
};

static struct gpio_buttons_platform_data homebox_gpio_buttons_platform_data = {
	.buttons = homebox_gpio_buttons,
	.nbuttons = ARRAY_SIZE(homebox_gpio_buttons),
	.poll_interval = 50,
};

static struct platform_device homebox_gpio_buttons_device = {
	.name = "gpio-buttons",
	.id = -1,
	.dev = {
		.platform_data = &homebox_gpio_buttons_platform_data,
	},
};

static void __init homebox_buttons_init(void)
{
	platform_device_register(&homebox_gpio_buttons_device);
}

#define DEFINE_LED(pin, color, func, trigger, activelow) \
               { .gpio = MXS_PIN_TO_GPIO(pin), \
                 .name = "homebox:" color ":" func, \
                 .default_trigger = trigger, .active_low = activelow }

static struct gpio_led homebox_leds[] = {
	DEFINE_LED(PINID_GPMI_RDY0, "red",    "error",  "none",       0),
	DEFINE_LED(PINID_GPMI_RDN,  "yellow", "status", "none",       0),
	DEFINE_LED(PINID_GPMI_ALE,  "green",  "power",  "default-on", 0),
};

static struct gpio_led_platform_data homebox_led_pdata = {
	.num_leds = ARRAY_SIZE(homebox_leds),
	.leds = homebox_leds,
};

static struct platform_device homebox_led_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &homebox_led_pdata,
	},
};

static void __init homebox_leds_init(void)
{
	platform_device_register(&homebox_led_device);
}

static struct spi_board_info homebox_spi_board_info[]= {
	{
		.modalias = "spidev",
		.max_speed_hz = 1 * 1000 * 1000,
		.bus_num = 1,
		.chip_select = 0,
		.mode = SPI_MODE_0,
	},
};

static struct i2c_board_info __initdata homebox_i2c_devices[] = {
	{
		I2C_BOARD_INFO("ds1337", 0x68),
	},
};

static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx28_set_input_clk(24000000, 24000000, 32000, 50000000);
}

static void __init homebox_init_machine(void)
{
	mx28_pinctrl_init();

	/* Init iram allocate */
#ifdef CONFIG_VECTORS_PHY_ADDR
	/* reserve the first page for irq vector table*/
	iram_init(MX28_OCRAM_PHBASE + PAGE_SIZE, MX28_OCRAM_SIZE - PAGE_SIZE);
#else
	iram_init(MX28_OCRAM_PHBASE, MX28_OCRAM_SIZE);
#endif

	mx28_gpio_init();
	tqma28l_pins_init();
	homebox_pins_init();

	mx28_device_init();

	homebox_leds_init();
	homebox_buttons_init();

	i2c_register_board_info(0, homebox_i2c_devices, ARRAY_SIZE(homebox_i2c_devices));

	spi_register_board_info(homebox_spi_board_info, ARRAY_SIZE(homebox_spi_board_info));
}

MACHINE_START(HOMEBOX, "IEQualize Home-Box")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx28_map_io,
	.init_irq	= mx28_irq_init,
	.init_machine	= homebox_init_machine,
	.timer		= &mx28_timer.timer,
MACHINE_END
