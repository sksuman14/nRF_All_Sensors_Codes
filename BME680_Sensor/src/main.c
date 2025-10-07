#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

#define NUM_SENSORS 1

void main(void)
{
	// Array of sensor devices
	const struct device *sensors[NUM_SENSORS] = {
		DEVICE_DT_GET(DT_NODELABEL(bme680_0)),
	};

	struct sensor_value temp, press, humidity, gas_res;

	// Check if all sensors are ready
	for (int i = 0; i < NUM_SENSORS; i++)
	{
		if (!device_is_ready(sensors[i]))
		{
			printk("Sensor %d: device not ready.\n", i);
			return;
		}
		printf("Sensor %d: Device %p name is %s\n", i, sensors[i], sensors[i]->name);
	}

	while (1)
	{
		// Loop through each sensor to fetch and display data
		for (int i = 0; i < NUM_SENSORS; i++)
		{
			sensor_sample_fetch(sensors[i]);
			sensor_channel_get(sensors[i], SENSOR_CHAN_AMBIENT_TEMP, &temp);
			sensor_channel_get(sensors[i], SENSOR_CHAN_PRESS, &press);
			sensor_channel_get(sensors[i], SENSOR_CHAN_HUMIDITY, &humidity);
			sensor_channel_get(sensors[i], SENSOR_CHAN_GAS_RES, &gas_res);

			printf("Sensor %d: T: %d.%02d; P: %d.%02d; H: %d.%02d; G: %d\n",
				   i, temp.val1, temp.val2 / 1000, press.val1, press.val2 / 1000,
				   humidity.val1, humidity.val2 / 1000, gas_res.val1 / 1000);
		}
		k_sleep(K_MSEC(1000));
	}
}
