#include <zephyr/logging/log.h>       // Include Zephyr's logging library
#include <zephyr/kernel.h>            // Include Zephyr's kernel library
#include <zephyr/drivers/led_strip.h> // Include the LED strip driver
#include <zephyr/device.h>            // Include Zephyr's device handling library
#include <zephyr/sys/util.h>          // Include Zephyr's utility functions library
#include <zephyr/sys/printk.h>        // Include printk for printing messages

LOG_MODULE_REGISTER(main); // Register the logging module for the main function

#define stripNode DT_ALIAS(ledstrip)                    // Define a symbolic name "stripNode" for the device tree alias "led_strip"
#define stripNumPixels DT_PROP(stripNode, chain_length) // Define a symbolic name "stripNumPixels" for the number of LEDs in the strip

/* Variables that can be changed according to convenience */
#define delayTime 20 // Define a delay time of 20 milliseconds for smooth updates

// Define the structure for RGB colors
struct ledRgb
{
    uint8_t r; // Red component (0-255)
    uint8_t g; // Green component (0-255)
    uint8_t b; // Blue component (0-255)
};

// Macro to define an RGB color
#define RGB(_r, _g, _b) \
    {                   \
        .r = (_r),      \
        .g = (_g),      \
        .b = (_b)}

// Define the color you want to blink (only one color at a time)
static struct ledRgb blinkingColor = RGB(0x00, 0x2f, 0x00); // Green color for blinking effect

// Define an array to hold the color values for each pixel on the strip
struct ledRgb pixels[stripNumPixels];

// Get the LED strip device from the device tree
static const struct device *const strip = DEVICE_DT_GET(stripNode);

// Function to adjust color brightness for breathing effect (blink)
void adjust_brightness(struct ledRgb *color, uint8_t brightness)
{
    color->r = (color->r * brightness) / 255;
    color->g = (color->g * brightness) / 255;
    color->b = (color->b * brightness) / 255;
}

int main(void)
{
    uint8_t brightness = 0; // Variable to control brightness (used for breathing effect)
    int brightnessStep = 5;  // The step for increasing/decreasing brightness

    // Check if the LED strip device is ready
    if (!device_is_ready(strip))
    {
        LOG_ERR("LED strip device not found"); // Log an error if the device is not found
        return -1;                             // Exit the program with an error code
    }

    LOG_INF("Found LED strip device %s", strip->name); // Log the name of the LED strip device

    // Main loop to continuously update the LED strip
    while (1)
    {
        // Clear the pixels array (turn off all LEDs)
        memset(pixels, 0x00, sizeof(pixels));

        // Apply the breathing effect on the color for all LEDs
        for (int ledIdx = 0; ledIdx < stripNumPixels; ledIdx++)
        {
            struct ledRgb currentColor = blinkingColor;
            adjust_brightness(&currentColor, brightness); // Apply brightness adjustment for blinking (breathing effect)
            memcpy(&pixels[ledIdx], &currentColor, sizeof(struct ledRgb));
        }

        // Update the LED strip with the new pixel data
        led_strip_update_rgb(strip, pixels, stripNumPixels);

        // Adjust brightness for blinking (breathing) effect (increase/decrease)
        brightness += brightnessStep;
        if (brightness == 0 || brightness == 255)
        {
            brightnessStep = -brightnessStep; // Reverse the brightness change direction to create blinking
        }

        // Wait for a short period before the next update
        k_msleep(delayTime);
    }

    return 0; // Return 0 indicating the program ended successfully
}
