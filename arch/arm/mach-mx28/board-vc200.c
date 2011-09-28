/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2011 IEQualize GmbH
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
#include <linux/i2c/at24.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/device.h>
#include <mach/pinctrl.h>

#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>

#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/leds.h>

#include "device.h"
#include "module-tqma28l.h"
#include "mx28_pins.h"
#include "board-vc200.h"

static struct gpio_keys_button vc200_gpio_keys[] = {
	{
		.type = EV_KEY,
		.code = KEY_F1,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_CS),
		.desc = "Reset (T4)",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F2,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_RESET),
		.desc = "Service (T1)",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F3,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_RD_E),
		.desc = "LON Service (T3)",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F4,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_RS),
		.desc = "PIN Switch (T2)",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F5,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_D08),
		.desc = "LON SVC", /* FT5000 LON SVC */
		.active_low = 1,
	},
};

static struct gpio_keys_platform_data vc200_gpio_keys_platform_data = {
	.buttons = vc200_gpio_keys,
	.nbuttons = ARRAY_SIZE(vc200_gpio_keys),
	.rep = 0,
};

static struct platform_device vc200_gpio_keys_device = {
	.name = "gpio-keys",
	.id = -1,
	.dev = {
		.platform_data = &vc200_gpio_keys_platform_data,
	},
};

static void __init vc200_keys_init(void)
{
	platform_device_register(&vc200_gpio_keys_device);
}

#define DEFINE_LED(pin, color, func) \
               { .gpio = MXS_PIN_TO_GPIO(pin), \
                 .name = "vc200:" color ":" func, \
                 .default_trigger = "none", .active_low = 1 }

static struct gpio_led vc200_leds[] = {
	DEFINE_LED(PINID_LCD_D09, "green",  "led1"),
	DEFINE_LED(PINID_LCD_D05, "green",  "led2_1"),
	DEFINE_LED(PINID_LCD_D06, "yellow", "led2_2"),
	DEFINE_LED(PINID_LCD_D07, "red",    "led2_3"),
	DEFINE_LED(PINID_LCD_D00, "green",  "led3_1"),
	DEFINE_LED(PINID_LCD_D01, "yellow", "led3_2"),
	DEFINE_LED(PINID_LCD_D02, "green",  "led4_1"),
	DEFINE_LED(PINID_LCD_D03, "red",    "led4_2"),
	DEFINE_LED(PINID_LCD_D04, "green",  "led5"),
};

static struct gpio_led_platform_data vc200_led_pdata = {
	.num_leds = ARRAY_SIZE(vc200_leds),
	.leds = vc200_leds,
};

static struct platform_device vc200_led_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &vc200_led_pdata,
	},
};

static void __init vc200_leds_init(void)
{
	platform_device_register(&vc200_led_device);
}

static struct spi_eeprom vc200_at25_pdata = {
	.byte_len = SZ_64K,
	.name = "m95512",
	.page_size = 128,
	.flags = EE_ADDR2,
};

static struct spi_board_info vc200_spi_board_info[] __initdata = {
	{
		.modalias = "at25",
		.max_speed_hz = 1 * 1000 * 1000,
		.bus_num = 1,
		.chip_select = 0,
		.platform_data = &vc200_at25_pdata,
	},
};

static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx28_set_input_clk(24000000, 24000000, 32000, 50000000);
}

static void __init vc200_init_machine(void)
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
	vc200_pins_init();

	mx28_device_init();

	vc200_leds_init();
	vc200_keys_init();

	spi_register_board_info(vc200_spi_board_info, ARRAY_SIZE(vc200_spi_board_info));
}

/* FIXME: "Freescale MX28" string is required for mfg updater tool */
MACHINE_START(VC200, "Freescale MX28 - TQMa28L - VC200")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx28_map_io,
	.init_irq	= mx28_irq_init,
	.init_machine	= vc200_init_machine,
	.timer		= &mx28_timer.timer,
MACHINE_END
