#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define RAIN_GPIO_PORT DT_NODELABEL(gpio0)
#define RAIN_PIN 4

static const struct device *rain_dev;

void main(void)
{   
	int ret;

	rain_dev = DEVICE_DT_GET(RAIN_GPIO_PORT);
	if (!device_is_ready(rain_dev))
	{
		printk("GPIO device not ready\n");
		return;
	}

	// Configure as DIGITAL INPUT with pull-up
	ret = gpio_pin_configure(rain_dev, RAIN_PIN, GPIO_INPUT | GPIO_PULL_UP);
	if (ret != 0)
	{
		printk("Failed to configure rain pin\n");
		return;
	}

	while (1)
	{
		int val = gpio_pin_get(rain_dev, RAIN_PIN);
		val = !val;
		// val will be 1 or 0 only
		printk("Rain pin value = %d\n", val);

		// If using ADC-based value (1000–3000) in another function:
		// if (val >= 1000 && val <= 3000) { tip_count++; }

		k_msleep(200);
	}
}
