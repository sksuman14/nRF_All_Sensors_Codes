#include <zephyr/kernel.h>        // Include Zephyr kernel APIs
#include <zephyr/device.h>        // Include Zephyr device driver model
#include <zephyr/drivers/gpio.h>  // Include GPIO driver APIs

#define SW0_NODE DT_ALIAS(sw0)    // Get the device-tree alias for button SW0

// Create GPIO specification for the button (pin, port, flags from device tree)
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

// Create GPIO specification for the LED (pin, port, flags from device tree)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)                   // Main function (program entry point)
{
    // Configure button GPIO pin as input
    gpio_pin_configure_dt(&button0, GPIO_INPUT);

    // Configure LED GPIO pin as output
    gpio_pin_configure_dt(&led0, GPIO_OUTPUT);

    while (1)                    // Infinite loop
    {
        // Read button state and set LED state accordingly
        // Button pressed  → LED ON
        // Button released → LED OFF
        gpio_pin_set_dt(&led0, gpio_pin_get_dt(&button0));
    }
}
