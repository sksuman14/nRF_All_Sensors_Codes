#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys_clock.h>

// Constants
#define TRIG_PIN DT_ALIAS(sw0)
#define ECHO_PIN DT_ALIAS(sw1)
#define TRIG_PULSE_US 10
#define TIMEOUT_US 30000
#define SOUND_SPEED_CM_PER_US 0.0343
#define MIN_DISTANCE_CM 2
#define MAX_DISTANCE_CM 400
#define MEASURE_INTERVAL_MS 500

// GPIO configurations
static const struct gpio_dt_spec trig = GPIO_DT_SPEC_GET_OR(TRIG_PIN, gpios, {0});
static const struct gpio_dt_spec echo = GPIO_DT_SPEC_GET_OR(ECHO_PIN, gpios, {0});

// Initialize the ultrasonic sensor
static inline bool ultrasonic_sensor_init(void) {
    if (!device_is_ready(trig.port) || !device_is_ready(echo.port)) {
        printk("Error: GPIO device(s) not ready\n");
        return false;
    }

    gpio_pin_configure_dt(&trig, GPIO_OUTPUT_INACTIVE); // Trigger as output
    gpio_pin_configure_dt(&echo, GPIO_INPUT);           // Echo as input
    k_sleep(K_MSEC(100));                               // Stabilize the sensor
    printk("Ultrasonic sensor initialized successfully\n");
    return true;
}

// Measure distance using the ultrasonic sensor
static inline double ultrasonic_get_distance(void) {
    uint32_t start_time, pulse_duration;

    // Trigger a 10µs pulse
    gpio_pin_set_dt(&trig, 1);
    k_busy_wait(TRIG_PULSE_US);
    gpio_pin_set_dt(&trig, 0);

    // Wait for echo pin to go high
    start_time = k_cycle_get_32();
    while (!gpio_pin_get_dt(&echo)) {
        if (k_cyc_to_us_floor32(k_cycle_get_32() - start_time) > TIMEOUT_US) {
            return -1; // Timeout
        }
    }

    // Measure pulse duration
    start_time = k_cycle_get_32();
    while (gpio_pin_get_dt(&echo)) {
        if (k_cyc_to_us_floor32(k_cycle_get_32() - start_time) > TIMEOUT_US) {
            return -1; // Timeout
        }
    }
    pulse_duration = k_cyc_to_us_floor32(k_cycle_get_32() - start_time);

    // Calculate distance
    double distance_cm = (pulse_duration * SOUND_SPEED_CM_PER_US) / 2.0;

    return (distance_cm >= MIN_DISTANCE_CM && distance_cm <= MAX_DISTANCE_CM) ? distance_cm : -2;
}

#endif // ULTRASONIC_H
