
// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/pwm.h>

// static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
// static const uint32_t min_pulse = DT_PROP(DT_NODELABEL(servo), min_pulse);
// static const uint32_t max_pulse = DT_PROP(DT_NODELABEL(servo), max_pulse);

// #define STEP PWM_USEC(100)

// enum direction {
// 	DOWN,
// 	UP,
// };

// int main(void)
// {
// 	uint32_t pulse_width = min_pulse;
// 	enum direction dir = UP;
// 	int ret;

// 	printk("Servomotor control\n");

// 	if (!pwm_is_ready_dt(&servo)) {
// 		printk("Error: PWM device %s is not ready\n", servo.dev->name);
// 		return 0;
// 	}

// 	while (1) {
// 		ret = pwm_set_pulse_dt(&servo, pulse_width);
// 		if (ret < 0) {
// 			printk("Error %d: failed to set pulse width\n", ret);
// 			return 0;
// 		}

// 		if (dir == DOWN) {
// 			if (pulse_width <= min_pulse) {
// 				dir = UP;
// 				pulse_width = min_pulse;
// 			} else {
// 				pulse_width -= STEP;
// 			}
// 		} else {
// 			pulse_width += STEP;

// 			if (pulse_width >= max_pulse) {
// 				dir = DOWN;
// 				pulse_width = max_pulse;
// 			}
// 		}

// 		// k_sleep(K_SECONDS(1));
// 		k_msleep(1000);
// 	}
// 	return 0;
// }


// /*Door open and close project */

// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/pwm.h>
// #include <zephyr/drivers/gpio.h>

// /* Servo configuration */
// static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
// static const uint32_t min_pulse = DT_PROP(DT_NODELABEL(servo), min_pulse);
// static const uint32_t max_pulse = DT_PROP(DT_NODELABEL(servo), max_pulse);
// #define STEP PWM_USEC(100)

// /* Button configuration */
// #define BUTTON_NODE DT_NODELABEL(button0)
// static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON_NODE, gpios, {0});
// static struct gpio_callback button_cb_data;

// enum door_state {
//     DOOR_CLOSED,
//     DOOR_OPEN,
// };

// static volatile enum door_state state = DOOR_CLOSED;

// /* Button interrupt handler */
// void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
// {
//     /* Toggle the door state */
//     if (state == DOOR_CLOSED) {
//         state = DOOR_OPEN;
//     } else {
//         state = DOOR_CLOSED;
//     }
// }

// void main(void)
// {
//     uint32_t pulse_width = min_pulse;
//     int ret;

//     printk("Servo motor door control with button input\n");

//     /* Initialize PWM */
//     if (!pwm_is_ready_dt(&servo)) {
//         printk("Error: PWM device %s is not ready\n", servo.dev->name);
//         return;
//     }

//     /* Initialize button */
//     if (!device_is_ready(button.port)) {
//         printk("Error: Button device not ready\n");
//         return;
//     }
//     ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
//     if (ret < 0) {
//         printk("Error %d: Failed to configure button pin\n", ret);
//         return;
//     }
//     ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
//     if (ret < 0) {
//         printk("Error %d: Failed to configure button interrupt\n", ret);
//         return;
//     }
//     gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
//     gpio_add_callback(button.port, &button_cb_data);

//     while (1) {
//         if (state == DOOR_OPEN) {
//             pulse_width = max_pulse; // Open position
//         } else {
//             pulse_width = min_pulse; // Closed position
//         }

//         ret = pwm_set_pulse_dt(&servo, pulse_width);
//         if (ret < 0) {
//             printk("Error %d: Failed to set pulse width\n", ret);
//         }

//         /* Sleep to debounce button presses and allow for smooth servo control */
//         k_msleep(50);
//     }
// }

// /*  individual buttons for opening and closing the door */

// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/pwm.h>
// #include <zephyr/drivers/gpio.h>

// static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
// static const uint32_t min_pulse = DT_PROP(DT_NODELABEL(servo), min_pulse);
// static const uint32_t max_pulse = DT_PROP(DT_NODELABEL(servo), max_pulse);

// // Button configurations
// #define BUTTON_OPEN_NODE DT_ALIAS(sw0)
// #define BUTTON_CLOSE_NODE DT_ALIAS(sw1)

// static const struct gpio_dt_spec button_open = GPIO_DT_SPEC_GET_OR(BUTTON_OPEN_NODE, gpios, {0});
// static const struct gpio_dt_spec button_close = GPIO_DT_SPEC_GET_OR(BUTTON_CLOSE_NODE, gpios, {0});

// void main(void)
// {
//     int ret;
//     uint32_t pulse_width = min_pulse;

//     printk("Servo Door Control with Individual Open and Close Buttons\n");

//     // Initialize PWM
//     if (!pwm_is_ready_dt(&servo)) {
//         printk("Error: PWM device %s is not ready\n", servo.dev->name);
//         return;
//     }

//     // Configure buttons
//     if (!device_is_ready(button_open.port)) {
//         printk("Error: Button device (open) %s is not ready\n", button_open.port->name);
//         return;
//     }
//     ret = gpio_pin_configure_dt(&button_open, GPIO_INPUT);
//     if (ret < 0) {
//         printk("Error %d: Failed to configure open button\n", ret);
//         return;
//     }

//     if (!device_is_ready(button_close.port)) {
//         printk("Error: Button device (close) %s is not ready\n", button_close.port->name);
//         return;
//     }
//     ret = gpio_pin_configure_dt(&button_close, GPIO_INPUT);
//     if (ret < 0) {
//         printk("Error %d: Failed to configure close button\n", ret);
//         return;
//     }

//     // Main control loop
//     while (1) {
//         if (gpio_pin_get_dt(&button_open) > 0) {
//             printk("Opening door\n");
//             pulse_width = max_pulse;
//             ret = pwm_set_pulse_dt(&servo, pulse_width);
//             if (ret < 0) {
//                 printk("Error %d: Failed to set pulse width for opening\n", ret);
//             }
//         }

//         if (gpio_pin_get_dt(&button_close) > 0) {
//             printk("Closing door\n");
//             pulse_width = min_pulse;
//             ret = pwm_set_pulse_dt(&servo, pulse_width);
//             if (ret < 0) {
//                 printk("Error %d: Failed to set pulse width for closing\n", ret);
//             }
//         }

//         k_msleep(100); // Debounce delay
//     }
// }

/// Pressing 

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

/* Servo configuration */
static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
static const uint32_t min_pulse = DT_PROP(DT_NODELABEL(servo), min_pulse);
static const uint32_t max_pulse = DT_PROP(DT_NODELABEL(servo), max_pulse);

/* Button configuration */
#define BUTTON_NODE DT_NODELABEL(button0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON_NODE, gpios, {0});

enum door_state {
    DOOR_CLOSED,
    DOOR_OPEN,
};

static volatile enum door_state state = DOOR_CLOSED;

void main(void)
{
    uint32_t pulse_width = min_pulse;
    int ret;

    printk("Servo motor door control with button hold functionality\n");
    /* Initialize button */
    if (!device_is_ready(button.port)) {
        printk("Error: Button device not ready\n");
        return;
    }
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        printk("Error %d: Failed to configure button pin\n", ret);
        return;
    }

    while (1) {
        /* Check button state */
        if (gpio_pin_get_dt(&button)) {
            state = DOOR_OPEN;  // Button pressed
        } else {
            state = DOOR_CLOSED;  // Button released
        }

        /* Update servo position based on state */
        if (state == DOOR_OPEN) {
            pulse_width = max_pulse; // Open position
        } else {
            pulse_width = min_pulse; // Closed position
        }

        ret = pwm_set_pulse_dt(&servo, pulse_width);
        if (ret < 0) {
            printk("Error %d: Failed to set pulse width\n", ret);
        }

        /* Small delay to avoid excessive CPU usage */
        k_msleep(50);
    }
}
