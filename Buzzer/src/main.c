#include <zephyr/zephyr.h>		 // Zephyr main header
#include <zephyr/drivers/gpio.h> // GPIO driver
#include <zephyr/device.h>		 // Device driver
#include <hal/nrf_gpio.h>		 // Nordic GPIO HAL

#define buzzer_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios) // Define buzzer pin (LED0)
#define SLEEP_TIME_MS 100								 // Define sleep time (1 second) 

void main(void)
{
	nrf_gpio_cfg_output(buzzer_PIN); // Configure buzzer pin as output

	while (1)
	{
		nrf_gpio_pin_set(buzzer_PIN);	// Turn buzzer on
		k_sleep(K_MSEC(SLEEP_TIME_MS)); // Delay for specified time

		nrf_gpio_pin_clear(buzzer_PIN);	// Turn buzzer off
		k_sleep(K_MSEC(SLEEP_TIME_MS)); // Delay for specified time
	}
}
