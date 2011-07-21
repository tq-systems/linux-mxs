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
#include <mach/device.h>
#include <mach/pinctrl.h>

#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>

#include "device.h"
#include "module-ieqma28.h"
#include "qx28.h"


#if defined(CONFIG_GPIO_PCA953X) || defined(CONFIG_GPIO_PCA953X_MODULE)
static struct pca953x_platform_data qx28_pca953x_pdata = {
	.gpio_base = -1,
};
#endif

#if defined(CONFIG_LEDS_PCA955X) || defined(CONFIG_LEDS_PCA955X_MODULE)

#define DEFINE_LED(id, chip, color, trigger) \
               [id] = { .name = chip ":" color ":led" #id, .default_trigger = trigger }

static struct led_info qx28_pca955x_leds[] = {
	DEFINE_LED(0, "ic201", "red",    "none"),
	DEFINE_LED(1, "ic201", "yellow", "none"),
	DEFINE_LED(2, "ic201", "green",  "none"),
	DEFINE_LED(3, "ic201", "yellow", "none"),
	DEFINE_LED(4, "ic201", "yellow", "none"),
	DEFINE_LED(5, "ic201", "yellow", "none"),
	DEFINE_LED(6, "ic201", "yellow", "none"),
	DEFINE_LED(7, "ic201", "yellow", "none"),
};

static struct led_platform_data qx28_pca955x_leds_pdata = {
	.num_leds = ARRAY_SIZE(qx28_pca955x_leds),
	.leds = qx28_pca955x_leds,
};
#endif

static struct i2c_board_info __initdata qx28_i2c_devices[] = {
#if defined(CONFIG_GPIO_PCA953X) || defined(CONFIG_GPIO_PCA953X_MODULE)
	{
		I2C_BOARD_INFO("pca9534", 0x20),
		.platform_data = &qx28_pca953x_pdata,
// TODO: .irq
	},
#endif
#if defined(CONFIG_RTC_DRV_PCF2127A) || defined(CONFIG_RTC_DRV_PCF2127A_MODULE)
	{
		I2C_BOARD_INFO("pcf2127a", 0x51),
	},
#endif
#if defined(CONFIG_LEDS_PCA955X) || defined(CONFIG_LEDS_PCA955X_MODULE)
	{
		I2C_BOARD_INFO("pca9551", 0x60),
		.platform_data = &qx28_pca955x_leds_pdata,
	},
#endif
};

static void __init i2c_device_init(void)
{
	i2c_register_board_info(0, qx28_i2c_devices, ARRAY_SIZE(qx28_i2c_devices));
}

#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_MTD_M25P80_MODULE)
static struct flash_platform_data mx28_spi_flash_data = {
	.name = "m25p80",
	.type = "w25x80",
};
#endif

static struct spi_board_info spi_board_info[] __initdata = {
#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_MTD_M25P80_MODULE)
	{
		/* the modalias must be the same as spi device driver name */
		.modalias = "m25p80", /* Name of spi_driver for this device */
		.max_speed_hz = 20000000,     /* max spi clock (SCK) speed in HZ */
		.bus_num = 1, /* Framework bus number */
		.chip_select = 0, /* Framework chip select. */
		.platform_data = &mx28_spi_flash_data,
	},
#endif
};

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
	/* Add qx28 special code */
	i2c_device_init();
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
