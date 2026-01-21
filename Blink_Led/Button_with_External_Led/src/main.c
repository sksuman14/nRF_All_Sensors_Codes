/* buttonWithExternalLed */

#include <zephyr/kernel.h>         // Kernel headers required for Zephyr
#include <zephyr/device.h>         // Device driver interface
#include <zephyr/drivers/gpio.h>   // GPIO drivers
#include <zephyr/sys/printk.h>     // Printk function for debugging output

#define delayTime 100          // Delay time in milliseconds between button checks

// Retrieve button configuration from the devicetree alias 'sw0'
#define SW0_NODE DT_ALIAS(sw0) // Define the devicetree node for the button (sw0 alias)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay) // Check if the button node is available and correctly configured
#error "Unsupported board: sw0 devicetree alias is not defined" // Throw an error if the button node is not defined
#endif

// GPIO configuration for the button and LED
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0}); // Initialize the button GPIO
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0}); // Initialize the LED GPIO

// Main function where the program execution begins
int main(void)
{
    int ret;

    // Check if the button GPIO is ready and configure it as an input pin
    if (!gpio_is_ready_dt(&button)) // Verify if the button GPIO is properly initialized
    {
        printk("Error: button device %s is not ready\n", button.port->name); // Print error if button GPIO is not ready
        return 0; // Exit the program if the button GPIO is not ready
    }
    gpio_pin_configure_dt(&button, GPIO_INPUT); // Configure the button pin as input

    // Check if the LED GPIO is ready and configure it as an output pin
    if (led.port && gpio_is_ready_dt(&led)) // Verify if the LED GPIO is properly initialized
    {
        gpio_pin_configure_dt(&led, GPIO_OUTPUT); // Configure the LED pin as output
        printk("Set up LED at %s pin %d\n", led.port->name, led.pin); // Print confirmation of successful LED setup
    }
    else
    {
        printk("Error: LED device %s is not ready; ignoring it\n", led.port->name); // Print error if LED GPIO is not ready
        return 0; // Exit the program if the LED GPIO is not ready
    }

    printk("Press the button to turn ON the LED\n"); // Inform the user that pressing the button will control the LED

    // Main loop: Continuously check the button state and control the LED accordingly
    while (1)
    {
        int val = gpio_pin_get_dt(&button); // Read the current state of the button
        if (val >= 0 && led.port) // If the button state is valid and the LED port is ready
        {
            gpio_pin_set_dt(&led, val); // Set the LED state to match the button state
        }
        k_msleep(delayTime); // Wait for a short delay before checking the button state again
    }

    return 0; // Return from main, though this line is not typically reached in an embedded system loop
}
