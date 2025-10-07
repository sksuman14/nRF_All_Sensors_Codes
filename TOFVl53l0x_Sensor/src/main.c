/* Time Of Flight Sensor VL53L0X */

#include <zephyr/kernel.h>  
#include <zephyr/device.h>  
#include <zephyr/drivers/sensor.h>  
#include <stdio.h> 
#include <zephyr/sys/printk.h>  
#include <zephyr/drivers/i2c.h>  

// Define a macro to retrieve Device Tree instance
#define DT_INST(inst, compat) UTIL_CAT(DT_N_INST, DT_DASH(inst, compat))

// Define a macro to retrieve the node label for the I2C device
#define I2C0_NODE DT_NODELABEL(st_vl53l0x)

// Main function
int main(void)
{
    // Declare and initialize a variable to hold the I2C device
	const struct device *const dev = DEVICE_DT_GET_ANY(st_vl53l0x);

    // Declare variables for sensor readings and return values
	struct sensor_value value;
	int ret;

    // Check if the device is ready
	if (!device_is_ready(dev)) {
        // Print a message if the device is not ready and return
		printk("sensor: device not ready.\n");
		return 0;
	}

    // Infinite loop for continuous sensor reading
	while (1) {
        // Fetch sensor samples from the device
		ret = sensor_sample_fetch(dev);
        
        // Check if fetching sensor samples failed
		if (ret) {
            // Print an error message if fetching samples failed and return
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return 0;
		}

        // Get proximity sensor data from the device
		ret = sensor_channel_get(dev, SENSOR_CHAN_PROX, &value);
        
        // Print the proximity sensor value
		printk("prox is %d\n", value.val1);

        // Get distance sensor data from the device
		ret = sensor_channel_get(dev, SENSOR_CHAN_DISTANCE, &value);

        // Print the distance sensor value in meters
		printf("distance is %.3fm\n", sensor_value_to_double(&value));

        // Convert meters to centimeters
		double distance_cm = sensor_value_to_double(&value) * 100.0;
        
        // Print the distance sensor value in centimeters
		printf("distance is %.3fcm\n", distance_cm);

        // Delay for 1 second before the next iteration
		k_sleep(K_MSEC(1000));
	}

    // End of main function
	return 0;
}
