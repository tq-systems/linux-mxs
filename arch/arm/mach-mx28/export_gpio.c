/*
 * Copyright (C) 2012 IEQualize GmbH
 *   Michael Heimpold <mhe@iequalize.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#include <linux/gpio.h>
#include "export_gpio.h"

int export_gpios(struct export_gpio *array, size_t num)
{
	int i, err;

	for (i = 0; i < num; i++, array++) {
		err = gpio_request_one(array->gpio, array->flags, array->label);
		if (err)
			goto err_free;

		err = gpio_export(array->gpio, array->direction_may_change);
		if (err) {
			gpio_free(array->gpio);
			goto err_free;
		}

		err = gpio_sysfs_set_active_low(array->gpio, array->active_low);
		if (err) {
			gpio_unexport(array->gpio);
			gpio_free(array->gpio);
			goto err_free;
		}
	}
	return 0;

err_free:
	while (i--) {
		gpio_unexport((--array)->gpio);
		gpio_free(array->gpio);
	}
	return err;
}
