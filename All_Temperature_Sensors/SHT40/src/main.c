#include <zephyr/kernel.h>         // Zephyr kernel header
#include <zephyr/device.h>         // Device API
#include <zephyr/drivers/i2c.h>    // I2C driver API
#include <zephyr/drivers/sensor.h> // Sensor driver API

#define sleepTime 1000 // Define sleep time of 1 second

void main(void)
{
  const struct device *sht = DEVICE_DT_GET_ANY(sensirion_sht4x); // Get the SHT4x device from the device tree

  if (!device_is_ready(sht)) // Check if the SHT device is ready
  {
    printf("Device %s is not ready\n", sht->name); // Print error if the device is not ready
    return;
  }

  struct sensor_value temp, hum; // Variables to store temperature and humidity values

  while (1) // Infinite loop for continuous operation
  {
    if (sensor_sample_fetch(sht) == 0) // Fetch sensor samples; if successful
    {
      sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp); // Get temperature value
      sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum);      // Get humidity value

      printf("Temperature: %.2f°C Humidity: %.2f %%RH\n",
             sensor_value_to_double(&temp), sensor_value_to_double(&hum)); // Print the sensor values
    }
    else
    {
      printf("Error: Failed to fetch sensor data\n"); // Print an error message if data fetch fails
    }

    k_msleep(sleepTime); // Sleep for the defined interval
  }
}
