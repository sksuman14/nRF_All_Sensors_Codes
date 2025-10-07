#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/mhz19b.h>
#include <zephyr/drivers/uart.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/dt-bindings/led/led.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>

#define DELAY_TIME K_MSEC(1000)

#define STRIP_NODE DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(led_strip), chain_length)

#define RGB(_r, _g, _b)                 \
    {                                   \
        .r = (_r), .g = (_g), .b = (_b) \
    }

#define LED_GAP 0

struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

static const struct led_rgb colors[] = {
    RGB(0xff, 0x00, 0x00),  /* red */
    RGB(0x00, 0x0ff, 0x00), /* green */
    RGB(0x00, 0x00, 0x0ff), /* blue */
    RGB(0xff, 0xaf, 0x00),  /*yellow*/
    RGB(0xff, 0x00, 0xff),  /*magenta*/
};

size_t cursor = 0, color = 0;
int rc;

bool once = true;

const struct device *const mhz19 = DEVICE_DT_GET_ANY(winsen_mhz19b);

/*GAS function*/
void gas_data()
{

    int ret;
    struct sensor_value val; // Sensor Readout Value

    /*Fetching MHZ19E data*/

    if (!device_is_ready(mhz19)) // Verify that a device is ready for use.
    {
        if (once)
        {
            once = false;
            sys_reboot();
        }
        // printk("sensor: device not found.\n");
        // return 0;
    }

    val.val1 = 5000;
    ret = sensor_attr_set(mhz19, SENSOR_CHAN_CO2, SENSOR_ATTR_FULL_SCALE, &val); // This is likely a function provided by sensor related library or fremework.
                                                                                 // It's being called with four arguments.
    if (ret != 0)
    {
        // printk("failed to set range to %d\n", val.val1);
        // return 0;
    }

    val.val1 = 1;
    ret = sensor_attr_set(mhz19, SENSOR_CHAN_CO2, SENSOR_ATTR_MHZ19B_ABC, &val); // dev – Pointer to the sensor device
                                                                                 // chan – The channel the attribute belongs to, if any. Some attributes may only be set for all channels of a device, depending on device capabilities.
                                                                                 // attr – The attribute to set
                                                                                 // val – The value to set the attribute to

    // if (ret != 0)
    // {
    //     // printk("failed to set ABC to %d\n", val.val1);
    //     // return 0;
    // }

    // printk("Reading configurations from sensor:\n");

    ret = sensor_attr_get(mhz19, SENSOR_CHAN_CO2, SENSOR_ATTR_FULL_SCALE, &val); // Get an attribute for a sensor

    // if (ret != 0)
    // {
    //     // printk("failed to get range\n");
    //     // return 0;
    // }
    // ret = sensor_attr_get(mhz19, SENSOR_CHAN_CO2, SENSOR_ATTR_MHZ19B_ABC, &val);
    data_fetch(val);
}

void data_fetch(struct sensor_value val)
{

    if (sensor_sample_fetch(mhz19) != 0)
    {
        printk("sensor: sample fetch fail.\n");
        return 0;
    }

    if (sensor_channel_get(mhz19, SENSOR_CHAN_CO2, &val) != 0)
    {
        printk("sensor: channel get fail.\n");
        return 0;
    }

    printk("sensor: co2 reading: %d\n", val.val1);
    if (val.val1 >= 4000)
    {
        memset(&pixels, 0x00, sizeof(pixels));
        for (int i = 0; i < 24;)
        {
            memcpy(&pixels[i], &colors[0], sizeof(struct led_rgb));
            i = i + 1 + LED_GAP;
        }
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    }
    else if (val.val1 >= 3000 && val.val1 < 4000)
    {
        memset(&pixels, 0x00, sizeof(pixels));
        for (int i = 0; i < 24;)
        {
            memcpy(&pixels[i], &colors[2], sizeof(struct led_rgb));
            i = i + 1 + LED_GAP;
        }
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    }
    else if (val.val1 >= 2000 && val.val1 < 3000)
    {
        memset(&pixels, 0x00, sizeof(pixels));
        for (int i = 0; i < 24;)
        {
            memcpy(&pixels[i], &colors[4], sizeof(struct led_rgb));
            i = i + 1 + LED_GAP;
        }
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    }
    else if (val.val1 >= 1000 && val.val1 < 2000)
    {
        memset(&pixels, 0x00, sizeof(pixels));
        for (int i = 0; i < 24;)
        {
            memcpy(&pixels[i], &colors[3], sizeof(struct led_rgb));
            i = i + 1 + LED_GAP;
        }
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    }
    else if (val.val1 >= 400 && val.val1 < 1000)
    {
        memset(&pixels, 0x00, sizeof(pixels));
        for (int i = 0; i < 24;)
        {
            memcpy(&pixels[i], &colors[1], sizeof(struct led_rgb));
            i = i + 1 + LED_GAP;
        }
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    }
    k_sleep(DELAY_TIME);
    gas_data();
}
int main(void)
{
    gas_data();
}