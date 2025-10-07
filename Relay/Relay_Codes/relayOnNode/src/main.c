#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <hal/nrf_gpio.h>

void main(void)
{
	while (1) {
		nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
		nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // Switch on the Relay
		k_sleep(K_MSEC(1000));

		nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // Switch off the Relay
		k_sleep(K_MSEC(1000));
	}
}