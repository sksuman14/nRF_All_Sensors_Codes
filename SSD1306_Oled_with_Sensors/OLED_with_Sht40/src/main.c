#include <zephyr/kernel.h>          // Kernel APIs
#include <zephyr/device.h>          // Device management APIs
#include <zephyr/drivers/display.h> // Display operations APIs
#include <zephyr/display/cfb.h>     // Character framebuffer APIs
#include <zephyr/drivers/sensor.h>  // Sensor operations APIs
#include <stdio.h>

#define SLEEP_DURATION_MS 1000      // Sleep duration between updates

// Function to update OLED display with sensor values
void update_display(const struct device *oled_dev, int lux, double temp, double hum, uint8_t font_height)
{
    char display_str[32];             // Buffer for display strings
    cfb_framebuffer_clear(oled_dev, false); // Clear display buffer

    snprintf(display_str, sizeof(display_str), "Lux: %d", lux);
    cfb_print(oled_dev, display_str, 0, 0); // Display Lux value

    snprintf(display_str, sizeof(display_str), "Temp: %.2fC", temp);
    cfb_print(oled_dev, display_str, 0, font_height); // Display temperature

    snprintf(display_str, sizeof(display_str), "Hum: %.2f%%", hum);
    cfb_print(oled_dev, display_str, 0, 2 * font_height); // Display humidity

    cfb_framebuffer_finalize(oled_dev); // Finalize display update
}

void main(void)
{
    // Get device bindings
    const struct device *oled_dev = device_get_binding("SSD1306");
    const struct device *vcnl = DEVICE_DT_GET_ANY(vishay_vcnl4040);
    const struct device *sht = DEVICE_DT_GET_ANY(sensirion_sht4x);

    // Check if OLED is ready and initialize
    if (!oled_dev || !device_is_ready(oled_dev))
    {
        printk("OLED Display initialization failed.\n");
        return;
    }
    if (display_set_pixel_format(oled_dev, PIXEL_FORMAT_MONO10) != 0 || cfb_framebuffer_init(oled_dev) != 0)
    {
        printk("OLED Display: Initialization failed\n");
        return;
    }

    cfb_framebuffer_clear(oled_dev, true);   // Clear display
    cfb_framebuffer_invert(oled_dev);        // Invert display colors
    display_blanking_off(oled_dev);          // Turn off display blanking

    // Get font size for text positioning
    uint8_t font_width, font_height;
    cfb_get_font_size(oled_dev, 0, &font_width, &font_height);

    int counter = 0;                         // Initialize counter
    while (1)
    {
        int lux = 0;                         // Default Lux value
        double temp = 0.0, hum = 0.0;        // Default temperature and humidity values

        // Fetch VCNL4040 sensor data if available
        if (device_is_ready(vcnl))
        {
            struct sensor_value lux_val;
            if (sensor_sample_fetch(vcnl) == 0 && sensor_channel_get(vcnl, SENSOR_CHAN_LIGHT, &lux_val) == 0)
            {
                lux = lux_val.val1;           // Store Lux value
            }
        }

        // Fetch SHT4x sensor data if available
        if (device_is_ready(sht))
        {
            struct sensor_value temp_val, hum_val;
            if (sensor_sample_fetch(sht) == 0)
            {
                if (sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp_val) == 0)
                {
                    temp = sensor_value_to_double(&temp_val); // Store temperature
                }
                if (sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum_val) == 0)
                {
                    hum = sensor_value_to_double(&hum_val);  // Store humidity
                }
            }
        }

        // Update OLED display with sensor values
        update_display(oled_dev, lux, temp, hum, font_height);

        // Log sensor data to console
        printk("Lux: %d, Temp: %.2f°C, Humidity: %.2f%%RH\n", lux, temp, hum);
        printk("Time: %d sec\n", counter++);

        k_msleep(SLEEP_DURATION_MS);         // Wait before next update
    }
}
