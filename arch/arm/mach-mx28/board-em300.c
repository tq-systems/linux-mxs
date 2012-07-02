/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2011-2012 IEQualize GmbH
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

#include <linux/leds.h>
#include <linux/leds-pca9532.h>

#include "device.h"
#include "module-tqma28l.h"
#include "mx28_pins.h"
#include "export_gpio.h"
#include "board-em300.h"

#define DEFINE_EM300DB_LED(id, color, func) \
	    { .name = "em300:" color ":" func, .state = PCA9532_OFF, .type = PCA9532_TYPE_LED }

static struct pca9532_platform_data em300_pca9532_1_pdata = {
	.leds = {
		DEFINE_EM300DB_LED(0,  "green",  "power"),
		DEFINE_EM300DB_LED(1,  "red",    "fault"),
		DEFINE_EM300DB_LED(2,  "green",  "link"),
		DEFINE_EM300DB_LED(3,  "yellow", "link"),
		DEFINE_EM300DB_LED(4,  "green",  "do_1"),
		DEFINE_EM300DB_LED(5,  "red",    "do_1"),
		DEFINE_EM300DB_LED(6,  "green",  "do_2"),
		DEFINE_EM300DB_LED(7,  "red",    "do_2"),
	},
	.psc = { 0, 0 },
	.pwm = { 0, 0 },
};

static struct pca9532_platform_data em300_pca9532_2_pdata = {
	.leds = {
		DEFINE_EM300DB_LED(0,  "green",  "di_1"),
		DEFINE_EM300DB_LED(1,  "red",    "di_1"),
		DEFINE_EM300DB_LED(2,  "green",  "di_2"),
		DEFINE_EM300DB_LED(3,  "red",    "di_2"),
		DEFINE_EM300DB_LED(4,  "green",  "di_3"),
		DEFINE_EM300DB_LED(5,  "red",    "di_3"),
		DEFINE_EM300DB_LED(6,  "green",  "di_4"),
		DEFINE_EM300DB_LED(7,  "red",    "di_4"),
		DEFINE_EM300DB_LED(8,  "green",  "di_5"),
		DEFINE_EM300DB_LED(9,  "red",    "di_5"),
		DEFINE_EM300DB_LED(10, "green",  "di_6"),
		DEFINE_EM300DB_LED(11, "red",    "di_6"),
		DEFINE_EM300DB_LED(12, "green",  "di_7"),
		DEFINE_EM300DB_LED(13, "red",    "di_7"),
		DEFINE_EM300DB_LED(14, "green",  "di_8"),
		DEFINE_EM300DB_LED(15, "red",    "di_8"),
	},
	.psc = { 0, 0 },
	.pwm = { 0, 0 },
};

static struct i2c_board_info em300_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pca9532", 0x60),
		.platform_data = &em300_pca9532_1_pdata,
	},
	{
		I2C_BOARD_INFO("pca9532", 0x61),
		.platform_data = &em300_pca9532_2_pdata,
	},
	{
		I2C_BOARD_INFO("ads7828", 0x48),
	},
};

static struct export_gpio em300_gpios[] = {
	/* pin id,                            flags,               description,        direction, active low */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D05),     GPIOF_OUT_INIT_LOW,  "AI 1 Pt500",               0, 0 }, /* GPIO  37 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D06),     GPIOF_OUT_INIT_LOW,  "AI 2 Pt500",               0, 0 }, /* GPIO  38 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D07),     GPIOF_OUT_INIT_LOW,  "AI 3 Pt500",               0, 0 }, /* GPIO  39 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D08),     GPIOF_OUT_INIT_LOW,  "AI 4 Pt500",               0, 0 }, /* GPIO  40 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D09),     GPIOF_OUT_INIT_LOW,  "AI 5 Pt500",               0, 0 }, /* GPIO  41 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D10),     GPIOF_OUT_INIT_LOW,  "AI 6 Pt500",               0, 0 }, /* GPIO  42 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D11),     GPIOF_OUT_INIT_LOW,  "AI 7 Pt500",               0, 0 }, /* GPIO  43 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D12),     GPIOF_OUT_INIT_LOW,  "AI 8 Pt500",               0, 0 }, /* GPIO  44 */

	{ MXS_PIN_TO_GPIO(PINID_LCD_D13),     GPIOF_OUT_INIT_LOW,  "AI 1 Voltage",             0, 0 }, /* GPIO  45 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D14),     GPIOF_OUT_INIT_LOW,  "AI 2 Voltage",             0, 0 }, /* GPIO  46 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D15),     GPIOF_OUT_INIT_LOW,  "AI 3 Voltage",             0, 0 }, /* GPIO  47 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D16),     GPIOF_OUT_INIT_LOW,  "AI 4 Voltage",             0, 0 }, /* GPIO  48 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D17),     GPIOF_OUT_INIT_LOW,  "AI 5 Voltage",             0, 0 }, /* GPIO  49 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D18),     GPIOF_OUT_INIT_LOW,  "AI 6 Voltage",             0, 0 }, /* GPIO  50 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D19),     GPIOF_OUT_INIT_LOW,  "AI 7 Voltage",             0, 0 }, /* GPIO  51 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D20),     GPIOF_OUT_INIT_LOW,  "AI 8 Voltage",             0, 0 }, /* GPIO  52 */

	{ MXS_PIN_TO_GPIO(PINID_LCD_D21),     GPIOF_OUT_INIT_LOW,  "AI 1 NTC",                 0, 0 }, /* GPIO  53 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D22),     GPIOF_OUT_INIT_LOW,  "AI 2 NTC",                 0, 0 }, /* GPIO  54 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DATA0),  GPIOF_OUT_INIT_LOW,  "AI 3 NTC",                 0, 0 }, /* GPIO  64 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DATA1),  GPIOF_OUT_INIT_LOW,  "AI 4 NTC",                 0, 0 }, /* GPIO  65 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DATA2),  GPIOF_OUT_INIT_LOW,  "AI 5 NTC",                 0, 0 }, /* GPIO  66 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DATA3),  GPIOF_OUT_INIT_LOW,  "AI 6 NTC",                 0, 0 }, /* GPIO  67 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_CMD),    GPIOF_OUT_INIT_LOW,  "AI 7 NTC",                 0, 0 }, /* GPIO  72 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DETECT), GPIOF_OUT_INIT_LOW,  "AI 8 NTC",                 0, 0 }, /* GPIO  73 */

	{ MXS_PIN_TO_GPIO(PINID_LCD_D23),     GPIOF_IN,            "Bus Address 0x01",         0, 1 }, /* GPIO  55 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_RESET),   GPIOF_IN,            "Bus Address 0x02",         0, 1 }, /* GPIO 126 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_RD_E),    GPIOF_IN,            "Bus Address 0x04",         0, 1 }, /* GPIO  56 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_WR_RWN),  GPIOF_IN,            "Bus Termination Switch",   0, 1 }, /* GPIO  57 */

	{ MXS_PIN_TO_GPIO(PINID_SAIF0_MCLK),  GPIOF_IN,            "Digital Input 1",          0, 1 }, /* GPIO 116 */
	{ MXS_PIN_TO_GPIO(PINID_SAIF0_LRCLK), GPIOF_IN,            "Digital Input 2",          0, 1 }, /* GPIO 117 */
	{ MXS_PIN_TO_GPIO(PINID_SAIF0_BITCLK), GPIOF_IN,           "Digital Input 3",          0, 1 }, /* GPIO 118 */
	{ MXS_PIN_TO_GPIO(PINID_SAIF0_SDATA0), GPIOF_IN,           "Digital Input 4",          0, 1 }, /* GPIO 119 */
	{ MXS_PIN_TO_GPIO(PINID_SSP2_SCK),    GPIOF_IN,            "Digital Input 5",          0, 1 }, /* GPIO  80 */
	{ MXS_PIN_TO_GPIO(PINID_SSP2_MOSI),   GPIOF_IN,            "Digital Input 6",          0, 1 }, /* GPIO  81 */
	{ MXS_PIN_TO_GPIO(PINID_SSP2_MISO),   GPIOF_IN,            "Digital Input 7",          0, 1 }, /* GPIO  82 */
	{ MXS_PIN_TO_GPIO(PINID_SSP2_SS0),    GPIOF_IN,            "Digital Input 8",          0, 1 }, /* GPIO  83 */

	{ MXS_PIN_TO_GPIO(PINID_SSP0_SCK),    GPIOF_OUT_INIT_LOW,  "Digital Output 1",         0, 0 }, /* GPIO  74 */
	{ MXS_PIN_TO_GPIO(PINID_GPMI_RESETN), GPIOF_OUT_INIT_LOW,  "Digital Output 2",         0, 0 }, /* GPIO  28 */

	{ MXS_PIN_TO_GPIO(PINID_PWM4),        GPIOF_DIR_OUT,       "Power LED Blinker",        0, 1 }, /* GPIO 125 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_CS),      GPIOF_OUT_INIT_HIGH, "Display Board Reset",      0, 1 }, /* GPIO  59 */
};

static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx28_set_input_clk(24000000, 24000000, 32000, 50000000);
}

static void __init em300_init_machine(void)
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
	em300_pins_init();

	mx28_device_init();

	i2c_register_board_info(0, em300_i2c_devices, ARRAY_SIZE(em300_i2c_devices));

	export_gpios(em300_gpios, ARRAY_SIZE(em300_gpios));
}

MACHINE_START(EM300, "Viessmann Erweiterungsmodul 300")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx28_map_io,
	.init_irq	= mx28_irq_init,
	.init_machine	= em300_init_machine,
	.timer		= &mx28_timer.timer,
MACHINE_END
