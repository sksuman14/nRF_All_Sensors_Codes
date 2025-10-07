#include <zephyr/kernel.h>        // Include kernel header for Zephyr RTOS
#include <zephyr/device.h>        // Include device management header for Zephyr
#include <zephyr/drivers/uart.h>  // Include UART driver header for Zephyr
#include <stdlib.h>               // Include standard library for general-purpose functions
#include <string.h>               // Include string handling functions
#include <limits.h>               // Include limits for data types like INT_MAX, etc.

static const struct device *uart_dev; // Declare a pointer to store UART device object

/* Function to initialize UART with specific settings */
void uart_init(void)
{
    // Get a reference to the UART device (UART_0 in this case)
    uart_dev = device_get_binding("UART_0"); // Example of setting UART device
    if (!device_is_ready(uart_dev))  // Check if the device is ready for use
    {
        printk("UART device not found!\n");  // Print an error message if UART device is not found
    }
    
    struct uart_config uart_cfg;  // Declare a structure to store UART configuration settings

    /* Set UART configuration parameters */
    uart_cfg.baudrate = 9600;                // Set baud rate to 9600
    uart_cfg.parity = UART_CFG_PARITY_NONE;  // Set no parity
    uart_cfg.stop_bits = UART_CFG_STOP_BITS_1; // Set 1 stop bit
    uart_cfg.data_bits = UART_CFG_DATA_BITS_8; // Set 8 data bits
    uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE; // No flow control

    /* Configure the UART device with the settings */
    int ret = uart_configure(uart_dev, &uart_cfg); // Apply the configuration to the UART device
    if (ret != 0) // Check if the configuration was successful
    {
        printk("Failed to configure UART: %d\n", ret);  // Print an error if configuration fails
    }
}

/* Function to send a command via UART */
void send_uart_command(const uint8_t *cmd, size_t length)
{
    // Loop over each byte of the command and send it using UART
    for (size_t i = 0; i < length; i++)
    {
        uart_poll_out(uart_dev, cmd[i]);  // Send the byte to UART
        k_msleep(1);                      // Small delay for each byte to avoid flooding the UART
    }
}

/* Function to receive data from UART */
int receive_uart_data(uint8_t *buf, size_t length, k_timeout_t timeout)
{
    size_t received_len = 0;  // Variable to keep track of how much data has been received
    int ret;                  // Variable to store the result of UART data reading
    uint32_t start_time = k_uptime_get(); // Record the start time for timeout checking

    // Loop until we receive the specified length of data
    while (received_len < length)
    {
        ret = uart_poll_in(uart_dev, &buf[received_len]);  // Try to read one byte from UART
        if (ret == 0)  // If data is successfully received
        {
            received_len++;  // Increment the received data counter
        }
        else if (ret == -1)  // If no data available
        {
            k_msleep(1);  // Small delay before retrying
            if (k_uptime_get() - start_time > timeout.ticks)  // Check if timeout has occurred
            {
                return -ETIMEDOUT;  // Return timeout error code if timeout exceeds
            }
        }
        else  // In case of another error (non-zero ret)
        {
            return ret;  // Return the error code
        }
    }
    return 0;  // Return 0 if all requested data is received
}

int main(void)
{
    uart_init(); // Initialize UART with specified settings

    /* Data Read/Write Command Frame
       Device address/Function code/Starting Address Hi/Starting Address Li/Quantity Hi/Quantity Li/CRC Hi/CRC Li */
    uint8_t query_read_temp_hum[8] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x0B};  // Command to query temperature and humidity data

    uint8_t rx_buf[9];  // Buffer to store received data from UART (9 bytes expected)

    // Main loop that continuously sends commands and receives data
    while (1)
    {
        k_msleep(1000);  // Sleep for 1 second before sending the command
        send_uart_command(query_read_temp_hum, sizeof(query_read_temp_hum));  // Send the command to read temperature and humidity

        int ret = receive_uart_data(rx_buf, sizeof(rx_buf), K_MSEC(100));  // Receive the data with a 100 ms timeout
        if (ret == 0)  // If the data was received successfully
        {
            // Combine the received bytes for temperature and humidity into float values
            float temp = ((rx_buf[3] << 8) | rx_buf[4]);  // Combine high and low bytes for temperature
            float humi = ((rx_buf[5] << 8) | rx_buf[6]); // Combine high and low bytes for humidity

            // Print the temperature and humidity in a human-readable format
            printf("Temperature : %.2f   Humidity: %.2f\n", temp / 100, humi / 100); // Display the data in °C and percentage
        }
        else
        {
            printk("UART receive error: %d\n", ret);  // Print an error message if receiving data fails
        }

        k_msleep(1000);  // Small delay before sending the next query
    }

    return 0;  // Return from main function (although this line is not reached in an infinite loop)
}
