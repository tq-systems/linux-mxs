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
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>

#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/gpio_buttons.h>
#include <linux/leds.h>

#include "device.h"
#include "module-ieqma28.h"
#include "mx28_pins.h"
#include "qx28.h"

#define QX28_LON28_GPIO(chip, pin) (168 + chip * 8 + pin)

#if defined(CONFIG_GPIO_PCA953X) || defined(CONFIG_GPIO_PCA953X_MODULE)
static struct pca953x_platform_data qx28_lon28_pca953x_1_pdata = {
	.gpio_base = QX28_LON28_GPIO(0, 0),
};

static struct pca953x_platform_data qx28_lon28_pca953x_2_pdata = {
	.gpio_base = QX28_LON28_GPIO(1, 0),
};

static struct pca953x_platform_data qx28_lon28_pca953x_3_pdata = {
	.gpio_base = QX28_LON28_GPIO(2, 0),
};
#endif

static struct i2c_board_info __initdata qx28_i2c_devices[] = {
/*
 * Attention: Keep this the first three entries or implement a smarter search below!
 */
#if defined(CONFIG_GPIO_PCA953X) || defined(CONFIG_GPIO_PCA953X_MODULE)
	/* LON28 */
	{
		I2C_BOARD_INFO("pca9554", 0x38),
		.platform_data = &qx28_lon28_pca953x_1_pdata,
	},
	{
		I2C_BOARD_INFO("pca9554", 0x39),
		.platform_data = &qx28_lon28_pca953x_2_pdata,
	},
	{
		I2C_BOARD_INFO("pca9554", 0x3a),
		.platform_data = &qx28_lon28_pca953x_3_pdata,
	},
#endif
#if defined(CONFIG_RTC_DRV_PCF2127A) || defined(CONFIG_RTC_DRV_PCF2127A_MODULE)
	/* TB96 (aka TERMINAL1) */
	{
		I2C_BOARD_INFO("pcf2127a", 0x51),
	},
#endif
#if defined(CONFIG_RTC_DRV_DS1307) || defined(CONFIG_RTC_DRV_DS1307_MODULE)
	/* QX96_ETH_USB_RTC */
	{
		I2C_BOARD_INFO("ds1340", 0x68),
	},
#endif
};

static void __init qx28_i2c_init(void)
{
#if defined(CONFIG_GPIO_PCA953X) || defined(CONFIG_GPIO_PCA953X_MODULE)
	struct i2c_board_info *p = &qx28_i2c_devices[1];
	int irq_pin = MXS_PIN_TO_GPIO(PINID_SAIF1_SDATA0);

	if (gpio_request(irq_pin, "pca9554 irq")) {
		gpio_direction_input(irq_pin);
		p->irq = gpio_to_irq(irq_pin);
		printk(KERN_INFO "Got GPIO%d for pca9554 (0x%02x) IRQ\n", irq_pin, p->addr);
	} else {
		printk(KERN_ERR "Failed to request GPIO%d for pca955a (0x%02x) IRQ\n", irq_pin, p->addr);
	}
#endif
	i2c_register_board_info(0, qx28_i2c_devices, ARRAY_SIZE(qx28_i2c_devices));
}

#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
static struct gpio_keys_button qx28_lon28_gpio_keys[] = {
	{
		.type = EV_KEY,
		.code = KEY_F1,
		.gpio = QX28_LON28_GPIO(1, 0),
		.desc = "Button A",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F2,
		.gpio = QX28_LON28_GPIO(1, 1),
		.desc = "Button B",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F3,
		.gpio = QX28_LON28_GPIO(1, 2),
		.desc = "LON Service",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F5,
		.gpio = QX28_LON28_GPIO(1, 3),
		.desc = "LON SVC", /* FT5000 LON SVC */
		.active_low = 1,
	}, {
		.type = EV_SW,
		.code = SW_DOCK,
		.gpio = QX28_LON28_GPIO(1, 4),
		.desc = "LON Termination", /* LON 150 Ohm Termination Switch */
		.active_low = 1,
	},
};

static struct gpio_keys_platform_data qx28_lon28_gpio_keys_platform_data = {
	.buttons = qx28_lon28_gpio_keys,
	.nbuttons = ARRAY_SIZE(qx28_lon28_gpio_keys),
	.rep = 0,
};

static struct platform_device qx28_lon28_gpio_keys_device = {
	.name = "gpio-keys",
	.id = -1,
	.dev = {
		.platform_data = &qx28_lon28_gpio_keys_platform_data,
	},
};
#endif

#if defined(CONFIG_INPUT_GPIO_BUTTONS) || defined(CONFIG_INPUT_GPIO_BUTTONS_MODULE)
static struct gpio_button qx28_lon28_gpio_buttons[] = {
	{
		.type = EV_KEY,
		.code = KEY_F1,
		.gpio = QX28_LON28_GPIO(1, 0),
		.desc = "Button A",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F2,
		.gpio = QX28_LON28_GPIO(1, 1),
		.desc = "Button B",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F3,
		.gpio = QX28_LON28_GPIO(1, 2),
		.desc = "LON Service",
		.active_low = 1,
	}, {
		.type = EV_KEY,
		.code = KEY_F5,
		.gpio = QX28_LON28_GPIO(1, 3),
		.desc = "LON SVC", /* FT5000 LON SVC */
		.active_low = 1,
	}, {
		.type = EV_SW,
		.code = SW_DOCK,
		.gpio = QX28_LON28_GPIO(1, 4),
		.desc = "LON Termination", /* LON 150 Ohm Termination Switch */
		.active_low = 1,
	},
};

static struct gpio_buttons_platform_data qx28_lon28_gpio_buttons_platform_data = {
	.buttons = qx28_lon28_gpio_buttons,
	.nbuttons = ARRAY_SIZE(qx28_lon28_gpio_buttons),
	.poll_interval = 50,
};


static struct platform_device qx28_lon28_gpio_buttons_device = {
	.name = "gpio-buttons",
	.id = -1,
	.dev = {
		.platform_data = &qx28_lon28_gpio_buttons_platform_data,
	},
};
#endif

static void __init qx28_lon28_keys_init(void)
{
//	platform_device_register(&qx28_lon28_gpio_keys_device);
	platform_device_register(&qx28_lon28_gpio_buttons_device);
}

#if defined(CONFIG_LEDS_GPIO) || defined(CONFIG_LEDS_GPIO_MODULE)

#define DEFINE_LED(pin, color, func) \
               { .gpio = QX28_LON28_GPIO(0, pin), \
                 .name = "qx28-lon28:" color ":" func, \
                 .default_trigger = "none", .active_low = 1 }

static struct gpio_led qx28_lon28_leds[] = {
	DEFINE_LED(0, "green",  "led1"),
	DEFINE_LED(1, "green",  "led3_1"),
	DEFINE_LED(2, "yellow", "led3_2"),
	DEFINE_LED(3, "green",  "led4_1"),
	DEFINE_LED(4, "red",    "led4_2"),
	DEFINE_LED(5, "green",  "led5"),
};

static struct gpio_led_platform_data qx28_lon28_led_pdata = {
	.num_leds = ARRAY_SIZE(qx28_lon28_leds),
	.leds = qx28_lon28_leds,
};

static struct platform_device qx28_lon28_led_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &qx28_lon28_led_pdata,
	},
};

static void __init qx28_lon28_leds_init(void)
{
	platform_device_register(&qx28_lon28_led_device);
}
#else
static inline void qx28_lon28_leds_init(void) {}
#endif

#if defined(CONFIG_EEPROM_AT25) || defined(CONFIG_EEPROM_AT25_MODULE)
static struct spi_eeprom qx28_lon28_at25_pdata = {
	.byte_len = SZ_64K / 8,
	.name = "at25640",
	.page_size = 32,
	.flags = EE_ADDR2,
};
#endif

static struct spi_board_info qx28_spi_board_info[] __initdata = {
#if defined(CONFIG_EEPROM_AT25) || defined(CONFIG_EEPROM_AT25_MODULE)
	{
		.modalias = "at25",
		.max_speed_hz = 5000000,
		.bus_num = 0,
		.chip_select = 0,
		.platform_data = &qx28_lon28_at25_pdata,
	},
#endif
};

static void qx28_spi_init(void)
{
	spi_register_board_info(qx28_spi_board_info, ARRAY_SIZE(qx28_spi_board_info));
}

static struct mtd_partition qx28_nand_partitions[] = {
	{
		.name	= "uboot",
		.offset	= 0,
		.size	= 4 * SZ_512K,
	},
	{
		.name	= "uboot-env",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= 1 * SZ_128K,
	},
	{
		.name	= "linux",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= 3 * SZ_1M,
	},
	{
		.name	= "rootfs",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= 1 * SZ_64M,
	},
	{
		.name	= "rootfs_data",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= 4 * SZ_1M,
	},
	{
		.name	= "data",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	},
};

static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx28_set_input_clk(24000000, 24000000, 32000, 50000000);
}

static void __init qx28_device_init(void)
{
	qx28_i2c_init();
	qx28_spi_init();
}

static void __init qx28_init_machine(void)
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
	ieqma28_pins_init();
	qx28_pins_init();

	mx28_register_nand_partitions(qx28_nand_partitions, ARRAY_SIZE(qx28_nand_partitions));
	mx28_device_init();
	qx28_device_init();

	qx28_lon28_leds_init();
	qx28_lon28_keys_init();
}

/* FIXME: "Freescale MX28" string is required for mfg updater tool */
MACHINE_START(QX28, "Freescale MX28 - IEQMa28 - QX28")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx28_map_io,
	.init_irq	= mx28_irq_init,
	.init_machine	= qx28_init_machine,
	.timer		= &mx28_timer.timer,
MACHINE_END
