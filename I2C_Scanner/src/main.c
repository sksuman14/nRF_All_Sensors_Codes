#include <zephyr/kernel.h>      // Zephyr kernel API
#include <zephyr/device.h>      // Zephyr device driver API
#include <zephyr/drivers/i2c.h> // I2C driver API
#include <zephyr/sys/printk.h>  // Printing utilities

// Define the I2C device using the label from the device tree
#define I2C0_NODE DT_NODELABEL(mysensor) // Update this to match your DTS file
static const struct i2c_dt_spec i2c_dev = I2C_DT_SPEC_GET(I2C0_NODE);

#define delayTime 1000

void main(void)
{
    // Check if the I2C device is ready
    if (!device_is_ready(i2c_dev.bus))
    {
        printk("I2C bus %s is not ready!\n", i2c_dev.bus->name);
        return; // Exit if the I2C bus is not ready
    }

    printk("Starting I2C scan...\n");

    while (1) // Infinite loop to continuously scan the I2C bus
    {
        bool deviceFound = false; // Flag to track if any device is found

        // Scan all possible I2C addresses (from 0x03 to 0x77)
        for (uint8_t addr = 0x03; addr <= 0x77; addr++)
        {
            uint8_t temp_data; // Buffer for temporary data
            struct i2c_msg msgs[1]; // Array to hold I2C message
            msgs[0].buf = &temp_data; // Set the buffer
            msgs[0].len = 1;          // Set the length of data to be transferred
            msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP; // Set message flags

            // Perform the I2C transfer
            int ret = i2c_transfer(i2c_dev.bus, &msgs[0], 1, addr);

            // If transfer was successful, an I2C device was found
            if (ret == 0)
            {
                printk("I2C device found at address 0x%02x\n\n", addr);
                deviceFound = true; // Set the flag to true
            }
        }

        // If no devices were found during the scan, print a message
        if (!deviceFound)
        {
            printk("No I2C devices found on the bus.\n");
        }
        else
        {
            printk("I2C scan complete.\n"); // If any device was found, print completion message
        }

        k_msleep(delayTime); // Delay for 1 second before the next scan
    }
}
