#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>

#define SLEEP_TIME_MS   1000  // 1 second sleep time
#define STS3X_I2C_ADDR  0x4A  // Define the I2C address for STS3x sensor
#define STS3X_MEASURE_CMD 0x240B  // Command for single-shot measurement with medium repeatability

#define I2C_NODE DT_NODELABEL(mysensor)  // The I2C node label for the sensor

// Function to perform the soft reset on the STS3x sensor
static int16_t sts3x_soft_reset(const struct i2c_dt_spec *i2c_spec) {
    uint8_t reset_command[2] = {0x30, 0xA2};  // Soft reset command
    return i2c_write_dt(i2c_spec, reset_command, sizeof(reset_command));
}

// Function to measure the temperature using single-shot mode
static int16_t measure_temperature(const struct i2c_dt_spec *i2c_spec, float *temperature) {
    uint8_t command[2] = {STS3X_MEASURE_CMD >> 8, STS3X_MEASURE_CMD & 0xFF};  // Measurement command
    uint8_t temp_data[3];  // Buffer to store temperature data (2 bytes + 1 byte CRC)

    // Write the command and read the temperature data
    int ret = i2c_write_read_dt(i2c_spec, command, sizeof(command), temp_data, sizeof(temp_data));
    if (ret != 0) {
        return ret;
    }

    uint16_t raw_temp = (temp_data[0] << 8) | temp_data[1];  // Combine the 2 bytes of temperature data
    *temperature = -45.0 + 175.0 * ((float)raw_temp / 65535.0);  // Convert raw data to temperature

    return 0;
}

void main(void) {
    int ret;  // Variable to store return values from I2C operations
    float temperature = 0.0;  // Variable to store the measured temperature

    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);  // I2C device specification

    // Check if the I2C bus is ready
    if (!device_is_ready(dev_i2c.bus)) {
        printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
        return;
    }

    // Perform a soft reset of the sensor
    ret = sts3x_soft_reset(&dev_i2c);
    if (ret != 0) {
        printk("Soft reset failed! Error: %d\n", ret);
        return;
    }

    // Delay to allow the sensor to reset
    k_msleep(100);

    while (1) {
        // Measure the temperature using single-shot mode
        ret = measure_temperature(&dev_i2c, &temperature);
        if (ret != 0) {
            printk("Temperature measurement failed! Error: %d\n", ret);
            return;
        }

        // Print the temperature in Celsius and Fahrenheit
        printk("Temperature in Celsius: %.2f °C\n", temperature);
        printk("Temperature in Fahrenheit: %.2f F\n", (temperature * 1.8) + 32);

        // Sleep for the defined time
        k_msleep(SLEEP_TIME_MS);
    }
}
