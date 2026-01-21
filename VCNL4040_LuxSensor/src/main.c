#include <zephyr/kernel.h>		   // Include Zephyr kernel APIs for scheduling and timing
#include <zephyr/device.h>		   // Include Zephyr device APIs for device management
#include <zephyr/drivers/sensor.h> // Include Zephyr sensor APIs for sensor operations

#define delayTime 1000 // Define the sleep duration in milliseconds

struct sensor_value val; // Declare a variable to hold sensor data



void main(void)
{
	const struct device *vcnl = DEVICE_DT_GET_ANY(vishay_vcnl4040); // Get a handle to the VCNL4040 sensor device from the device tree

	if (!device_is_ready(vcnl)) // Check if the VCNL4040 sensor device is ready for use
	{
		printk("Sensor: device not ready.\n"); // Print an error message if the sensor device is not ready
		return;								   // Exit the function as the sensor is not available
	}

	while (1) // Enter an infinite loop to continuously fetch and display sensor data
	{
		printk("Starting VCNL4040 for Lux\n"); // Print a message to indicate the start of a new sensor reading

		if (sensor_sample_fetch(vcnl) == 0) // Fetch the latest sample from the VCNL4040 sensor
		{
			if (sensor_channel_get(vcnl, SENSOR_CHAN_LIGHT, &val) == 0) // Get the light intensity value from the sensor
			{
				printk(" Light (lux): %d\n", val.val1); // Print the current time (in seconds) and the light intensity in lux
			}
			else
			{
				printk("Sensor read error.\n"); // Print an error message if getting the sensor value failed
			}
		}
		else
		{
			printk("Sensor sample fetch error.\n"); // Print an error message if fetching the sensor sample failed
		}

		k_msleep(delayTime); // Sleep for the defined duration before the next iteration
		
	}
}
