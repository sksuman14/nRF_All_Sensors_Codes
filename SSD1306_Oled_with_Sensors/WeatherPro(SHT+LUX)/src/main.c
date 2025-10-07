#include <zephyr/kernel.h>          // Kernel APIs
#include <zephyr/device.h>          // Device management APIs
#include <zephyr/drivers/display.h> // Display operations APIs
#include <zephyr/display/cfb.h>     // Character framebuffer APIs
#include <zephyr/drivers/sensor.h>  // Sensor operations APIs
#include <stdio.h>

#define SLEEP_DURATION_MS 1000

void main(void)
{
    // Get device handles
    const struct device *oled_dev = device_get_binding("SSD1306");
    const struct device *vcnl = DEVICE_DT_GET_ANY(vishay_vcnl4040);
    const struct device *sht = DEVICE_DT_GET_ANY(sensirion_sht4x);

    // Check if devices are ready
    if (!oled_dev || !device_is_ready(vcnl) || !device_is_ready(sht))
    {
        printk("Device initialization failed.\n");
        return;
    }

    // Initialize OLED display
    if (display_set_pixel_format(oled_dev, PIXEL_FORMAT_MONO10) != 0 || cfb_framebuffer_init(oled_dev) != 0)
    {
        printk("OLED Display: Initialization failed\n");
        return;
    }

    cfb_framebuffer_clear(oled_dev, true);
    cfb_framebuffer_invert(oled_dev);
    display_blanking_off(oled_dev);

    // Get font size
    uint8_t font_width, font_height;
    cfb_get_font_size(oled_dev, 0, &font_width, &font_height);

    struct sensor_value lux_val, temp_val, hum_val;
    char display_str[32];
    int counter = 0;

    while (1) 
    {
        // Fetch sensor data
        if (sensor_sample_fetch(vcnl) == 0 && sensor_channel_get(vcnl, SENSOR_CHAN_LIGHT, &lux_val) == 0 &&
            sensor_sample_fetch(sht) == 0)
        {
            sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp_val);
            sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum_val);

            // Update OLED display
            cfb_framebuffer_clear(oled_dev, false);

            snprintf(display_str, sizeof(display_str), "Lux: %d", lux_val.val1);
            cfb_print(oled_dev, display_str, 0, 0);

            snprintf(display_str, sizeof(display_str), "Temp: %.2fC", sensor_value_to_double(&temp_val));
            cfb_print(oled_dev, display_str, 0, font_height);

            snprintf(display_str, sizeof(display_str), "Hum: %.2f%%", sensor_value_to_double(&hum_val));
            cfb_print(oled_dev, display_str, 0, 2 * font_height);

            cfb_framebuffer_finalize(oled_dev);

            // Log sensor data
            printk("Lux: %d, Temp: %.2f°C, Humidity: %.2f%%RH\n",
                   lux_val.val1, sensor_value_to_double(&temp_val), sensor_value_to_double(&hum_val));
        }
        else
        {
            printk("Error: Failed to fetch sensor data\n");
        }

        printk("Time: %d sec\n", counter++);
        k_msleep(SLEEP_DURATION_MS);
    }
}
