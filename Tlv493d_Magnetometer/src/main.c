#include <zephyr/zephyr.h>      // Zephyr core header
#include <zephyr/device.h>      // Device structure header
#include <zephyr/devicetree.h>  // Device tree macros
#include <zephyr/drivers/i2c.h> // I2C driver API
#include <zephyr/sys/printk.h>  // Printk for console output
#include <stdio.h>              // Standard IO functions

#define SLEEP_TIME_MS 500 // 1 second sleep time
#define I2C_NODE DT_NODELABEL(mysensor) // The I2C node label for the sensor
#define TLV493D_I2C_ADDR 0x5E // Adjust based on actual sensor wiring

// Function to configure TLV493D to active mode
static int tlv493d_set_active_mode(const struct i2c_dt_spec *i2c_spec) {
    // Send the appropriate command to configure the sensor to active mode
    uint8_t config_command[] = {0x00}; // Replace with correct bytes from the datasheet
    int ret = i2c_write_dt(i2c_spec, config_command, sizeof(config_command));
    if (ret) {
        printk("Failed to configure TLV493D to active mode. Error: %d\n", ret);
        return ret;
    }
    return 0;
}

// Function to read magnetic field data
static int read_magnetic_field(const struct i2c_dt_spec *i2c_spec, float *x, float *y, float *z) {
    uint8_t read_command[] = {0x11}; // Replace with the correct register address for XYZ data
    uint8_t mag_data[7]; // Read 7 bytes from the sensor to cover X, Y, Z, and temperature

    // Read 7 bytes of magnetic data
    int ret = i2c_write_read_dt(i2c_spec, read_command, sizeof(read_command), mag_data, sizeof(mag_data));
    if (ret) {
        printk("Failed to read magnetic data. Error: %d\n", ret);
        return ret;
    }

    // Raw data extraction; adjust based on sensor output format
    int16_t raw_x = (int16_t)(((uint16_t)mag_data[0] << 8) | (mag_data[1] & 0xF8)) >> 3;
    int16_t raw_y = (int16_t)(((uint16_t)mag_data[2] << 8) | (mag_data[3] & 0xF8)) >> 3;
    int16_t raw_z = (int16_t)(((uint16_t)mag_data[4] << 8) | (mag_data[5] & 0xF8)) >> 3;

    printk("Raw data: %02x %02x %02x %02x %02x %02x\n",
        mag_data[0], mag_data[1], mag_data[2], mag_data[3], mag_data[4], mag_data[5]);

    // Assuming a conversion factor (check datasheet for precise values)
    *x = raw_x * 0.098f; // Example conversion, adjust as needed
    *y = raw_y * 0.098f;
    *z = raw_z * 0.098f;

    return 0;
}

void main(void) {
    int ret;
    float x = 0.0, y = 0.0, z = 0.0;

    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

    // Check if the I2C bus is ready
    if (!device_is_ready(dev_i2c.bus)) {
        printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
        return;
    }

    // Set the sensor to active mode
    ret = tlv493d_set_active_mode(&dev_i2c);
    if (ret != 0) {
        return;
    }

    k_msleep(100); // Allow sensor time to stabilize

    while (1) {
        // Read the magnetic field data
        ret = read_magnetic_field(&dev_i2c, &x, &y, &z);
        if (ret != 0) {
            printk("Magnetic field measurement failed! Error: %d\n", ret);
            k_msleep(SLEEP_TIME_MS);
            continue;
        }

        // Print magnetic field values
        printk("Magnetic Field (µT) - X: %.2f, Y: %.2f, Z: %.2f\n", x, y, z);

        k_msleep(SLEEP_TIME_MS);
    }
}
