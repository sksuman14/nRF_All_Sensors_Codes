#include <zephyr/kernel.h>          // Zephyr kernel APIs for scheduling and timing
#include <zephyr/device.h>          // Zephyr device management APIs
#include <zephyr/drivers/display.h> // Zephyr display operations APIs
#include <zephyr/display/cfb.h>     // Zephyr character framebuffer APIs for OLED
#include <zephyr/drivers/sensor.h>  // Zephyr sensor operations APIs
#include <stdio.h>

/* Sleep duration in milliseconds */
#define SLEEP_DURATION_MS 1000
int counter=0;

void main(void)
{
    // Get device handles
    const struct device *oled_dev = device_get_binding("SSD1306");  // OLED display device
    const struct device *vcnl = DEVICE_DT_GET_ANY(vishay_vcnl4040); // VCNL4040 sensor
    struct sensor_value val;                                        // Variable to hold sensor data

    // Initialize OLED display
    if (!oled_dev)
    {
        printk("OLED Display: Device not found\n"); // Error if OLED not found
        return;
    }

    // Set pixel format and initialize framebuffer
    if (display_set_pixel_format(oled_dev, PIXEL_FORMAT_MONO10) != 0 || cfb_framebuffer_init(oled_dev) != 0)
    {
        printk("OLED Display: Initialization failed\n"); // Error if initialization fails
        return;
    }

    // Clear display and set properties
    cfb_framebuffer_clear(oled_dev, true);
    cfb_framebuffer_invert(oled_dev);
    display_blanking_off(oled_dev);

    // Get font size and display dimensions
    uint8_t font_width, font_height;
    cfb_get_font_size(oled_dev, 0, &font_width, &font_height);
    uint16_t display_width = cfb_get_display_parameter(oled_dev, CFB_DISPLAY_WIDTH);
    uint16_t display_height = cfb_get_display_parameter(oled_dev, CFB_DISPLAY_HEIGH);

    // Check if sensor is ready
    if (!device_is_ready(vcnl))
    {
        printk("Sensor: device not ready.\n"); // Error if sensor not ready
        return;
    }

    // Finalize initial framebuffer setup
    cfb_framebuffer_finalize(oled_dev);

    while (1) // Continuous loop to fetch and display sensor data
    {
        if (sensor_sample_fetch(vcnl) == 0) // Fetch sensor sample
        {
            if (sensor_channel_get(vcnl, SENSOR_CHAN_LIGHT, &val) == 0) // Get light intensity
            {
                // Prepare string for lux value
                char lux_str[16];
                snprintf(lux_str, sizeof(lux_str), "Lux: %d", val.val1);

                // Clear area where lux value will be displayed
                cfb_framebuffer_clear(oled_dev, false);

                // Calculate position for lux value
                uint16_t value_width = strlen(lux_str) * font_width;
                uint16_t value_start_x = (display_width - value_width) / 2;
                uint16_t value_start_y = (display_height - font_height) / 2;

                // Print lux value on display
                if (cfb_print(oled_dev, lux_str, value_start_x, value_start_y))
                {
                    printk("OLED Display: Failed to print Lux value\n"); // Error if printing fails
                }

                // Finalize framebuffer update
                cfb_framebuffer_finalize(oled_dev);
                printk("Time %d Sec\n",counter);
                counter++;
            }
            else
            {
                printk("Sensor read error.\n"); // Error if sensor read fails
            }
        }
        else
        {
            printk("Sensor sample fetch error.\n"); // Error if sample fetch fails
        }

        k_msleep(SLEEP_DURATION_MS); // Wait before next update
    }
}
