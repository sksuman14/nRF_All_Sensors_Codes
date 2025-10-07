#include <stdio.h>
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>

void main(void) {
    // Device initialization
    const struct device *display_dev = device_get_binding("SSD1306");
    if (!display_dev || !device_is_ready(display_dev)) {
        printk("Display device not ready\n");
        return;
    }

    const struct device *lis2dh = DEVICE_DT_GET_ANY(st_lis2dh);
    if (!lis2dh || !device_is_ready(lis2dh)) {
        printk("LIS2DH sensor not found or not ready\n");
        return;
    }

    // Initialize the character framebuffer
    if (cfb_framebuffer_init(display_dev)) {
        printk("Framebuffer initialization failed\n");
        return;
    }

    if (display_set_pixel_format(display_dev, PIXEL_FORMAT_MONO10) != 0) {
        printk("Failed to set required pixel format\n");
        return;
    }

    display_blanking_off(display_dev);

    uint8_t smallest_font_idx = 0; // Default to the first font
    uint8_t smallest_font_width = 13;
    uint8_t smallest_font_height = 13;
    cfb_set_kerning(display_dev, -1); // Positive value for larger gaps
    

    // Adjust coordinates for manual multiline alignment
    int line_spacing = smallest_font_height + 1; // Add a gap between lines
    int start_x = 2; // Starting X coordinate
    int start_y = 0; // Starting Y coordinate for the first line

    struct sensor_value accel[3]; // Sensor data storage
    char text_line[30]; // Buffer for individual text lines

    while (1) {
        // Clear the framebuffer
        cfb_framebuffer_clear(display_dev, false);

        // Fetch sensor data
        if (sensor_sample_fetch(lis2dh) == 0) {
            sensor_channel_get(lis2dh, SENSOR_CHAN_ACCEL_XYZ, accel);

            // Print "Accelerometer" label
            snprintf(text_line, sizeof(text_line), "Accelerometer");
            if (cfb_print(display_dev, text_line, start_x, start_y) != 0) {
                printk("Failed to print on the display\n");
                return;
            }

            // Print X-axis data
            snprintf(text_line, sizeof(text_line), "X: %.2f", sensor_value_to_double(&accel[0]));
            if (cfb_print(display_dev, text_line, start_x, start_y + line_spacing) != 0) {
                printk("Failed to print on the display\n");
                return;
            }

            // Print Y-axis data
            snprintf(text_line, sizeof(text_line), "Y: %.2f", sensor_value_to_double(&accel[1]));
            if (cfb_print(display_dev, text_line, start_x, start_y + 2 * line_spacing) != 0) {
                printk("Failed to print on the display\n");
                return;
            }

            // Print Z-axis data
            snprintf(text_line, sizeof(text_line), "Z: %.2f", sensor_value_to_double(&accel[2]));
            if (cfb_print(display_dev, text_line, start_x, start_y + 3 * line_spacing) != 0) {
                printk("Failed to print on the display\n");
                return;
            }

            cfb_framebuffer_finalize(display_dev); // Finalize the display update
        } else {
            printk("Failed to fetch sensor data\n");
        }

        // Wait for a short time before the next update
        k_sleep(K_MSEC(500));
    }
}
