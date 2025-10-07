#include <zephyr/zephyr.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define delayTime 200 // Sleep duration in milliseconds

// Fetch and display accelerometer data
static void fetch_and_display(const struct device *sensor)
{
	static unsigned int count = 0; // Sample counter
	struct sensor_value accel[3];  // Array to hold XYZ acceleration values
	size_t upTime=k_uptime_get_32();
	upTime=upTime/1000;

	if (sensor_sample_fetch(sensor) < 0)
	{ // Fetch data from sensor
		printf("ERROR: Sample fetch failed\n");
		return;
	}

	if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel) < 0)
	{ // Get XYZ acceleration data
		printf("ERROR: Channel get failed\n");
		return;
	}

	// Display sample count, uptime, and XYZ acceleration values
	printf("#%u @ %u ms: x %f, y %f, z %f\n",
		   ++count, upTime,
		   sensor_value_to_double(&accel[0]),
		   sensor_value_to_double(&accel[1]),
		   sensor_value_to_double(&accel[2]));
}

void main(void)
{
	// Get sensor device binding
	const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);

	if (!device_is_ready(sensor))
	{ // Check if sensor is ready
		printf("Device %s is not ready or not found\n", sensor ? sensor->name : "NULL");
		return;
	}

	while (1)
	{
		fetch_and_display(sensor); // Fetch and display sensor data
		k_msleep(delayTime);		   // Sleep for specified duration
	}
}
