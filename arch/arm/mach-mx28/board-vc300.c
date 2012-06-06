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
#include <linux/i2c/at24.h>
#include <linux/i2c/pca953x.h>

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
#include <linux/gpio_buttons.h>
#include <linux/leds.h>
#include <linux/leds-pca9532.h>
#include <linux/serial_sc16is7x2.h>

#include "device.h"
#include "module-tqma28l.h"
#include "mx28_pins.h"
#include "export_gpio.h"
#include "board-vc300.h"

#define VC300DB_GPIO(pin) (168 + SC16IS7X2_NR_GPIOS + (pin))

static struct pca953x_platform_data vc300_pca953x_pdata = {
	.gpio_base = VC300DB_GPIO(0),
};

#define DEFINE_VC300DB_LED(id, color, func) \
	    { .name = "vc300:" color ":" func, .state = PCA9532_OFF, .type = PCA9532_TYPE_LED }

static struct pca9532_platform_data vc300_pca9532_pdata = {
	.leds = {
		DEFINE_VC300DB_LED(0,  "green",  "led4_1"),
		DEFINE_VC300DB_LED(1,  "red",    "led4_2"),
		DEFINE_VC300DB_LED(2,  "green",  "led3_1"),
		DEFINE_VC300DB_LED(3,  "yellow", "led3_2"),
		DEFINE_VC300DB_LED(4,  "green",  "do"),
		DEFINE_VC300DB_LED(5,  "green",  "di_1"),
		DEFINE_VC300DB_LED(6,  "red",    "di_1"),
		DEFINE_VC300DB_LED(7,  "green",  "di_2"),
		DEFINE_VC300DB_LED(8,  "red",    "di_2"),
		DEFINE_VC300DB_LED(9,  "green",  "m-bus"),
	},
	.psc = { 0, 0 },
	.pwm = { 0, 0 },
};

static struct i2c_board_info vc300_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pca9554", 0x21),
		.platform_data = &vc300_pca953x_pdata,
		.irq = -1, /* setup dynamically */
	},
	{
		I2C_BOARD_INFO("pca9532", 0x61),
		.platform_data = &vc300_pca9532_pdata,
	},
};

static void vc300_i2c_init(void)
{
	struct i2c_board_info *p = &vc300_i2c_devices[0];
	int irq_pin = MXS_PIN_TO_GPIO(PINID_LCD_RS);

	if (gpio_request(irq_pin, "pca9554 irq") == 0) {
		gpio_direction_input(irq_pin);
		p->irq = gpio_to_irq(irq_pin);
		printk(KERN_INFO "Got GPIO%d for pca9554 (0x%02x) IRQ\n", irq_pin, p->addr);
	} else {
		printk(KERN_ERR "Failed to request GPIO%d for pca955a (0x%02x) IRQ\n", irq_pin, p->addr);
	}

	i2c_register_board_info(0, vc300_i2c_devices, ARRAY_SIZE(vc300_i2c_devices));
}

#define DEFINE_VC300DB_KEY(pin, ev_code, descr) \
	{					\
		.type = EV_KEY,			\
		.code = ev_code,		\
		.gpio = VC300DB_GPIO(pin),	\
		.desc = descr,			\
		.active_low = 1,		\
	}

static struct gpio_button vc300_gpio_buttons[] = {
	DEFINE_VC300DB_KEY(0, KEY_UP,    "Up"),
	DEFINE_VC300DB_KEY(1, KEY_RIGHT, "Right"),
	DEFINE_VC300DB_KEY(2, KEY_ENTER, "Ok"),
	DEFINE_VC300DB_KEY(3, KEY_LEFT,  "Left"),
	DEFINE_VC300DB_KEY(4, KEY_DOWN,  "Down"),
	DEFINE_VC300DB_KEY(5, KEY_ESC,   "Back"),
	{
		.type = EV_KEY,
		.code = KEY_F5,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_D08),
		.desc = "LON SVC", /* FT5000 LON SVC */
		.active_low = 1,
	},
	{
		.type = EV_KEY,
		.code = KEY_F6,
		.gpio = MXS_PIN_TO_GPIO(PINID_LCD_D07),
		.desc = "M-Bus Interrupt",
		.active_low = 1,
	},
};

static struct gpio_buttons_platform_data vc300_gpio_buttons_platform_data = {
	.buttons = vc300_gpio_buttons,
	.nbuttons = ARRAY_SIZE(vc300_gpio_buttons),
	.poll_interval = 50,
};

static struct platform_device vc300_gpio_buttons_device = {
	.name = "gpio-buttons",
	.id = -1,
	.dev = {
		.platform_data = &vc300_gpio_buttons_platform_data,
	},
};

static void __init vc300_buttons_init(void)
{
	platform_device_register(&vc300_gpio_buttons_device);
}

#define DEFINE_VC300MB_LED(pin, color, func, activelow) \
               { .gpio = MXS_PIN_TO_GPIO(pin), \
                 .name = "vc300:" color ":" func, \
                 .default_trigger = "none", .active_low = activelow }

static struct gpio_led vc300_leds[] = {
	DEFINE_VC300MB_LED(PINID_LCD_D09, "green", "led1", 1),
};

static struct gpio_led_platform_data vc300_led_pdata = {
	.num_leds = ARRAY_SIZE(vc300_leds),
	.leds = vc300_leds,
};

static struct platform_device vc300_led_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &vc300_led_pdata,
	},
};

static void __init vc300_leds_init(void)
{
	platform_device_register(&vc300_led_device);
}

static struct spi_eeprom vc300_at25_pdata = {
	.byte_len = SZ_64K,
	.name = "m95512",
	.page_size = 128,
	.flags = EE_ADDR2,
};

static struct sc16is7x2_platform_data vc300_sc16is7x2_pdata = {
	.uartclk = 14745600,
	.uart_base = 6,
	.uart_flags = { SC16IS7X2_UARTF_RS485 | SC16IS7X2_UARTF_RTSN,
	                SC16IS7X2_UARTF_RS485 | SC16IS7X2_UARTF_RTSN },
	.gpio_base = 168,
};

static struct spi_board_info vc300_spi_board_info[] __initdata = {
	{
		.modalias = "at25",
		.max_speed_hz = 1 * 1000 * 1000,
		.bus_num = 1,
		.chip_select = 0,
		.platform_data = &vc300_at25_pdata,
	},
	{
		.modalias = "sc16is7x2",
		.max_speed_hz = 10 * 1000 * 1000,
		.bus_num = 2,
		.chip_select = 0,
		.platform_data = &vc300_sc16is7x2_pdata,
		.irq = -1, /* setup dynamically */
	},
};

static void vc300_spi_init(void)
{
	struct spi_board_info *p = &vc300_spi_board_info[1];
	int irq_pin = MXS_PIN_TO_GPIO(PINID_SSP0_DATA1);

	if (gpio_request(irq_pin, "sc16is7x2 irq") == 0) {
		gpio_direction_input(irq_pin);
		p->irq = gpio_to_irq(irq_pin);
		printk(KERN_INFO "Got GPIO%d for sc16is7x2 IRQ\n", irq_pin);
	} else {
		printk(KERN_ERR "Failed to request GPIO%d for sc16is7x2 IRQ\n", irq_pin);
	}

	spi_register_board_info(vc300_spi_board_info, ARRAY_SIZE(vc300_spi_board_info));
}

static struct export_gpio vc300_gpios[] = {
	/* pin id,                            flags,               description,        direction, active low */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D06)   ,  GPIOF_OUT_INIT_LOW,  "M-Bus Power Switch",       0, 0 }, /* GPIO  38 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D10),     GPIOF_OUT_INIT_HIGH, "Linux SVC",                0, 1 }, /* GPIO  42 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D11),     GPIOF_OUT_INIT_LOW,  "Linux On",                 0, 0 }, /* GPIO  43 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D12),     GPIOF_OUT_INIT_HIGH, "FT5000 Reset",             0, 1 }, /* GPIO  44 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D20),     GPIOF_IN,            "Modem Detection 0x01",     0, 0 }, /* GPIO  52 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D21),     GPIOF_IN,            "Modem Detection 0x02",     0, 0 }, /* GPIO  53 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_WR_RWN),  GPIOF_OUT_INIT_LOW,  "Modem Power Switch",       0, 0 }, /* GPIO  57 */
	{ MXS_PIN_TO_GPIO(PINID_SSP0_DETECT), GPIOF_IN,            "Digital Input 1",          0, 1 }, /* GPIO  73 */
	{ MXS_PIN_TO_GPIO(PINID_GPMI_RESETN), GPIOF_IN,            "Digital Input 2",          0, 1 }, /* GPIO  28 */
	{ MXS_PIN_TO_GPIO(PINID_PWM3) ,       GPIOF_OUT_INIT_LOW,  "Digital Output",           0, 0 }, /* GPIO 124 */
	{ MXS_PIN_TO_GPIO(PINID_PWM4),        GPIOF_DIR_OUT,       "Power LED Blinker",        0, 1 }, /* GPIO 125 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_RD_E),    GPIOF_OUT_INIT_LOW,  "SW SPI CLK",               0, 0 }, /* GPIO  56 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_D23),     GPIOF_OUT_INIT_LOW,  "SW SPI MOSI",              0, 0 }, /* GPIO  55 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_RESET),   GPIOF_IN,            "SW SPI MISO",              0, 0 }, /* GPIO 126 */
	{ MXS_PIN_TO_GPIO(PINID_LCD_CS),      GPIOF_OUT_INIT_HIGH, "LCD Reset",                0, 0 }, /* GPIO  59 */
};

static struct platform_device vc300_gpio_dev_device = {
	.name = "GPIODEV",
	.id = -1,
};

static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx28_set_input_clk(24000000, 24000000, 32000, 50000000);
}

static void __init vc300_init_machine(void)
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
	vc300_pins_init();

	mx28_device_init();

	vc300_i2c_init();
	vc300_leds_init();
	vc300_buttons_init();
	vc300_spi_init();

	export_gpios(vc300_gpios, ARRAY_SIZE(vc300_gpios));

	platform_device_register(&vc300_gpio_dev_device);
}

MACHINE_START(VC300, "Viessmann Vitocom 300")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx28_map_io,
	.init_irq	= mx28_irq_init,
	.init_machine	= vc300_init_machine,
	.timer		= &mx28_timer.timer,
MACHINE_END
