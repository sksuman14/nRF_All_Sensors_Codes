#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h> // GPIO driver API
#include <zephyr/sys_clock.h>    // System clock utilities
#include "ultrasonic.h"

void main(void)
{
        if (!ultrasonic_sensor_init())
        {
                printk("Ultrasonic sensor initialization failed\n");
                return;
        }

        printk("Starting distance measurement...\n");
        while (1)
        {
                double distance = ultrasonic_get_distance();
                printk("Measured Distance: %.2f cm\n", distance);
                k_sleep(K_MSEC(MEASURE_INTERVAL_MS)); // Wait before next measurement
        }
}