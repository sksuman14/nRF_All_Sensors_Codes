/* WS2812BreathingLED - A program to control WS2812 LEDs  */

#include <zephyr/logging/log.h>          // Include Zephyr's logging library
#include <zephyr/kernel.h>               // Include Zephyr's kernel library
#include <zephyr/drivers/led_strip.h>    // Include the LED strip driver
#include <zephyr/device.h>               // Include Zephyr's device handling library
#include <zephyr/sys/util.h>             // Include Zephyr's utility functions library

// Macros for LED strip configuration
#define STRIP_NODE DT_ALIAS(led_strip)                              // Device tree alias for the LED strip
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(led_strip), chain_length) // Number of pixels in the LED strip

/* Variables that can be changed according to convenience */
#define delayTime 500 // Delay time in milliseconds
#define ledGap 0 // Gap between LEDs (if any)

// Macro to create an RGB color structure
#define RGB(_r, _g, _b) \
    {                   \
        .r = (_r),      \
        .g = (_g),      \
        .b = (_b)       \
    }

// Array to hold the pixel data for the LED strip
static struct led_rgb pixels[STRIP_NUM_PIXELS];

// Pointer to the LED strip device
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

// Array of colors to cycle through
static const struct led_rgb colors[] = {
    RGB(0x2f, 0x00, 0x00), /* red */
    RGB(0x00, 0x2f, 0x00), /* green */
    RGB(0x00, 0x00, 0x2f), /* blue */
    RGB(0x00, 0x2f, 0x2f), /* cyan */
    RGB(0x2f, 0x00, 0x2f), /* magenta */
    RGB(0x2f, 0x2f, 0x00), /* yellow */
};

int main(void)
{
    // Check if the LED strip device is ready
    if (!device_is_ready(strip))
    {
        printk("LED strip device not found\n");
    }

    printk("Found LED strip device %s\n", strip->name);

    size_t cursor = 0;   // Initialize the cursor to track the position in the LED strip
    size_t colorIdx = 0; // Initialize the color index to track the current color

    while (1)
    {
        // Clear the pixel buffer to remove any previous data
        memset(pixels, 0x00, sizeof(pixels));

        // Loop through the LED strip and set the color for each pixel
        for (int ledIdx = 0; ledIdx < STRIP_NUM_PIXELS;)
        {
            memcpy(&pixels[ledIdx], &colors[colorIdx], sizeof(struct led_rgb)); // Copy the current color to the pixel buffer
            ledIdx += (1 + ledGap);                                             // Move to the next pixel, considering any gap between LEDs
        }

        // Update the LED strip with the new pixel data
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);

        // Increment the cursor and check if it has reached the end of the strip
        cursor += STRIP_NUM_PIXELS;
        if (cursor >= STRIP_NUM_PIXELS)
        {
            cursor = 0;                                     // Reset the cursor if it exceeds the number of pixels
            colorIdx = (colorIdx + 1) % ARRAY_SIZE(colors); // Move to the next color, wrapping around if needed
        }

        k_msleep(delayTime); // Wait for a short delay before updating the strip again
    }
}
