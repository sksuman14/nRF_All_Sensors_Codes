/*
 * Copyright (c) 2021 G-Technologies Sdn. Bhd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/mhz19b.h>

int main(void)
{
	const struct device *dev;
	struct sensor_value val;
	int ret;

	printk("Winsen MH-Z19B CO2 sensor application\n");

	dev = DEVICE_DT_GET_ONE(winsen_mhz19b);
	if (!device_is_ready(dev)) {
		printk("sensor: device not found.\n");
		return 0;
	}

	while (1) {
		if (sensor_sample_fetch(dev) != 0) {
			printk("sensor: sample fetch fail.\n");
			return 0;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_CO2, &val) != 0) {
			printk("sensor: channel get fail.\n");
			return 0;
		}

		printk("sensor: co2 reading: %d\n", val.val1);

		k_msleep(1000);
	}
	return 0;
}
