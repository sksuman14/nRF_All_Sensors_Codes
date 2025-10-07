#include <zephyr/kernel.h>                // Include the Zephyr kernel header for system-level functions
#include <zephyr/device.h>                // Include the Zephyr device management header
#include <zephyr/drivers/uart.h>          // Include the Zephyr UART driver header
#include <stdlib.h>                        // Include the standard library header for general-purpose functions
#include <string.h>                        // Include the standard library header for string manipulation
#include <limits.h>                        // Include the standard library header for limits like INT_MAX, etc.

static const struct device *uart_dev;   // Declare a pointer to store UART device object

/* Function to initialize UART with specific settings */
void uart_init(void)
{
    // Get a reference to the UART device (UART_0 in this case)
    uart_dev = device_get_binding("UART_0"); // Example of setting UART device to "UART_0"
    
    // Check if the device is ready for use
    if (!device_is_ready(uart_dev))          // If the UART device isn't ready, handle the error
    {
        printk("UART device not found!\n"); // Print an error message if UART device is not found
    }
    
    // Declare a struct to hold UART configuration
    struct uart_config uart_cfg;

    /* Set UART configuration parameters */
    uart_cfg.baudrate = 9600;             // Set baud rate to 9600
    uart_cfg.parity = UART_CFG_PARITY_NONE; // No parity bits (even, odd, or none)
    uart_cfg.stop_bits = UART_CFG_STOP_BITS_1; // Set stop bits to 1
    uart_cfg.data_bits = UART_CFG_DATA_BITS_8; // Set data bits to 8 (standard for most UART devices)
    uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE; // No flow control (e.g., no RTS/CTS)

    /* Configure the UART device with the settings */
    int ret = uart_configure(uart_dev, &uart_cfg); // Apply the configuration to the UART device
    if (ret != 0)  // Check if configuring the UART failed
    {
        printk("Failed to configure UART: %d\n", ret); // Print an error message if configuration fails
    }
}

/* Function to send a command via UART */
void send_uart_command(const uint8_t *cmd, size_t length)
{
    // Loop through each byte of the command
    for (size_t i = 0; i < length; i++)
    {
        uart_poll_out(uart_dev, cmd[i]);   // Send one byte at a time over UART
        k_msleep(1); // Add a small delay between sending each byte (helps in case of high-speed UART)
    }
}

/* Function to receive data from UART */
int receive_uart_data(uint8_t *buf, size_t length, k_timeout_t timeout)
{
    size_t received_len = 0;   // Variable to track how many bytes we've received
    int ret;                    // Variable to store the return value of uart_poll_in
    uint32_t start_time = k_uptime_get();  // Get the current time (used to track timeout)

    // Loop until we receive the required number of bytes
    while (received_len < length)
    {
        ret = uart_poll_in(uart_dev, &buf[received_len]);  // Try to read one byte from UART
        if (ret == 0)  // If data was successfully received
        {
            received_len++;  // Increment the count of received bytes
        }
        else if (ret == -1)  // If no data was available (timeout condition)
        {
            k_msleep(1);  // Sleep for a small amount of time before retrying
            if (k_uptime_get() - start_time > timeout.ticks)  // Check if the timeout period has been exceeded
            {
                return -ETIMEDOUT;  // If timeout, return the error code for timeout
            }
        }
        else  // If any other error occurred (e.g., invalid data)
        {
            return ret;  // Return the error code
        }
    }
    return 0;  // Return 0 when the data has been successfully received
}

int main(void)
{
    uart_init(); // Initialize UART with specified settings

    uint8_t query_command[3] = {0xA5, 0x09, 0xAE}; // Define the query command to be sent (3 bytes)
    uint8_t rx_buf[9];  // Buffer to store received data (expected to be 9 bytes)

    while (1)  // Main loop, keeps running indefinitely
    {
        send_uart_command(query_command, sizeof(query_command)); // Send the query command to the UART device

        // Try to receive data from UART, expecting 9 bytes and a timeout of 100ms
        int ret = receive_uart_data(rx_buf, sizeof(rx_buf), K_MSEC(100)); 
        if (ret == 0)  // If data was successfully received
        {
            // Extract the 32-bit lux value from the received data (rx_buf[4] to rx_buf[7])
            uint32_t lux = (rx_buf[4] << 24) | (rx_buf[5] << 16) | (rx_buf[6] << 8) | rx_buf[7];
            
            // Convert lux value to float and scale it by dividing by 100
            uint32_t luxValue = lux / 100.0;  // Convert to lux value and scale

            // Print the lux value (light intensity) to the console
            printf("Light intensity: %d\n", luxValue);
        }
        else  // If an error occurred while receiving data
        {
            printk("UART receive error: %d\n", ret);  // Print the error code
        }

        k_msleep(1000);  // Sleep for 1000 ms (1 second) before sending the next query
    }

    return 0;  // The program will never reach here due to the infinite loop in main
}
