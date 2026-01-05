
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <stdint.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include <stdlib.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/sys/poweroff.h>

#include <string.h>

/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_NODELABEL(uart2)

#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

#define LED3_NODE DT_ALIAS(led3)
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

#define MSG_SIZE 64

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

void load_switch_on(void)
{
        gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
        gpio_pin_set_dt(&led3, 1);
		k_msleep(100);
        printf("Load switch: Power ON");
}

void load_switch_off(void)
{
        gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
        gpio_pin_set_dt(&led3, 0);
        printf("Load switch: Power OFF");
}

void gsm_enable(void)
{
		gpio_pin_configure_dt(&led1, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
		gpio_pin_set_dt(&led1, 1);
		k_msleep(100);
		printf("GSM Module: ENABLED");
}


void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	if (!uart_irq_update(uart_dev))
	{
		return;
	}

	if (!uart_irq_rx_ready(uart_dev))
	{
		return;
	}

	while (uart_fifo_read(uart_dev, &c, 1) == 1)
	{
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0)
		{
			rx_buf[rx_buf_pos] = '\0';

			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			rx_buf_pos = 0;
		}
		else if (rx_buf_pos < (sizeof(rx_buf) - 1))
		{
			rx_buf[rx_buf_pos++] = c;
		}
	}
}

void print_uart(uint8_t *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++)
	{
		uart_poll_out(uart_dev, buf[i]);
		printk("%d", buf[i]);
	}
}

int main(void)
{
	load_switch_on();
	k_msleep(1000);
	// gsm_enable();
	k_msleep(1000);

	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found!");
		return 0;
	}

	/* configure interrupt and callback to receive data */
	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0)
	{
		if (ret == -ENOTSUP)
		{
			printk("Interrupt-driven UART API support not enabled\n");
		}
		else if (ret == -ENOSYS)
		{
			printk("UART device does not support interrupt-driven API\n");
		}
		else
		{
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}
	uart_irq_rx_enable(uart_dev);
	printk("UART echo bot started.\r\n");
	while (1)
	{
		// Print raw buffer
		for (size_t i = 0; i < sizeof(rx_buf); i++)
		{
			if (rx_buf[i] != '\0')
				printk("%c", rx_buf[i]);
			else
				printk(" ");
		}
		printk("\n");

		/* -------------------- FIND "WS," -------------------- */
		int ws_start = -1;
		for (int i = 0; i < sizeof(rx_buf) - 3; i++)
		{
			if (rx_buf[i] == 'W' && rx_buf[i+1] == 'S' && rx_buf[i+2] == ',')
			{
				ws_start = i + 3;   // skip "WS,"
				break;
			}
		}

		/* -------------------- FIND WIND SPEED END (',') -------------------- */
		int ws_end = ws_start;
		while (rx_buf[ws_end] != ',' && rx_buf[ws_end] != '\0')
			ws_end++;

		/* -------------------- FIND "WD," -------------------- */
		int wd_start = -1;
		for (int i = 0; i < sizeof(rx_buf) - 3; i++)
		{
			if (rx_buf[i] == 'W' && rx_buf[i+1] == 'D' && rx_buf[i+2] == ',')
			{
				wd_start = i + 3;   // skip "WD,"
				break;
			}
		}

		/* -------------------- FIND WIND DIRECTION DECIMAL -------------------- */
		int dot = wd_start;
		while (rx_buf[dot] != '.' && rx_buf[dot] != '\0')
			dot++;

		/* -------------------- PRINT WIND SPEED -------------------- */
		printk("Wind Speed: ");
		for (int i = ws_start; i < ws_end; i++)
			printk("%c", rx_buf[i]);
		printk(" m/s\n");

		/* -------------------- PRINT WIND DIRECTION (1 DECIMAL) -------------------- */
		printk("Wind Direction: ");
		for (int i = wd_start; i < dot; i++)
			printk("%c", rx_buf[i]);

		// Print one decimal digit
		printk(".%c°\n", rx_buf[dot+1]);

		k_msleep(5000);
	}
	// return 0;
}


// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/uart.h>
// #include <zephyr/kernel.h>
// #include <zephyr/storage/disk_access.h>
// #include <zephyr/sys/printk.h>
// #include <string.h>
// #include <stdint.h>
// #include <zephyr/logging/log.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/uart.h>
// #include <stdio.h>
// #include <zephyr/drivers/gpio.h>
// #include <stdlib.h>
// #include <zephyr/drivers/sensor.h>
// #include <zephyr/sys/reboot.h>
// #include <zephyr/sys/poweroff.h>

// #include <string.h>

// /* change this to any other UART peripheral if desired */
// #define UART_DEVICE_NODE DT_NODELABEL(uart2)

// #define LED1_NODE DT_ALIAS(led1)
// static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

// #define LED3_NODE DT_ALIAS(led3)
// static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

// #define MSG_SIZE 64

// /* queue to store up to 10 messages (aligned to 4-byte boundary) */
// K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

// static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

// /* receive buffer used in UART ISR callback */
// static char rx_buf[MSG_SIZE];
// static int rx_buf_pos;

// void load_switch_on(void)
// {
//         gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
//         gpio_pin_set_dt(&led3, 1);
// 		k_msleep(100);
//         printf("Load switch: Power ON");
// }

// void load_switch_off(void)
// {
//         gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
//         gpio_pin_set_dt(&led3, 0);
//         printf("Load switch: Power OFF");
// }

// void gsm_enable(void)
// {
// 		gpio_pin_configure_dt(&led1, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
// 		gpio_pin_set_dt(&led1, 1);
// 		k_msleep(100);
// 		printf("GSM Module: ENABLED");
// }

// void gsm_disable(void)
// {
// 		gpio_pin_configure_dt(&led1, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
// 		gpio_pin_set_dt(&led1, 0);
// 		k_msleep(100);
// 		printf("GSM Module: DISABLED");
// }


// void serial_cb(const struct device *dev, void *user_data)
// {
// 	uint8_t c;

// 	if (!uart_irq_update(uart_dev))
// 	{
// 		return;
// 	}

// 	if (!uart_irq_rx_ready(uart_dev))
// 	{
// 		return;
// 	}

// 	while (uart_fifo_read(uart_dev, &c, 1) == 1)
// 	{
// 		if ((c == '\n' || c == '\r') && rx_buf_pos > 0)
// 		{
// 			rx_buf[rx_buf_pos] = '\0';

// 			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

// 			rx_buf_pos = 0;
// 		}
// 		else if (rx_buf_pos < (sizeof(rx_buf) - 1))
// 		{
// 			rx_buf[rx_buf_pos++] = c;
// 		}
// 	}
// }

// void print_uart(uint8_t *buf)
// {
// 	int msg_len = strlen(buf);

// 	for (int i = 0; i < msg_len; i++)
// 	{
// 		uart_poll_out(uart_dev, buf[i]);
// 		printk("%d", buf[i]);
// 	}
// }

// int main(void)
// {
// 	load_switch_on();
// 	k_msleep(1000);

// 	if (!device_is_ready(uart_dev))
// 	{
// 		printk("UART device not found!");
// 		return 0;
// 	}

// 	/* configure interrupt and callback to receive data */
// 	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

// 	if (ret < 0)
// 	{
// 		if (ret == -ENOTSUP)
// 		{
// 			printk("Interrupt-driven UART API support not enabled\n");
// 		}
// 		else if (ret == -ENOSYS)
// 		{
// 			printk("UART device does not support interrupt-driven API\n");
// 		}
// 		else
// 		{
// 			printk("Error setting UART callback: %d\n", ret);
// 		}
// 		return 0;
// 	}
// 	uart_irq_rx_enable(uart_dev);
// 	printk("UART echo bot started.\r\n");
	
// 	while (1)
// 	{
// 		// Enable GSM for 20 seconds and read values
// 		gsm_enable();
// 		int64_t gsm_on_end = k_uptime_get() + 20000;
		
// 		while (k_uptime_get() < gsm_on_end)
// 		{
// 			// Print raw buffer
// 			for (size_t i = 0; i < sizeof(rx_buf); i++)
// 			{
// 				if (rx_buf[i] != '\0')
// 					printk("%c", rx_buf[i]);
// 				else
// 					printk(" ");
// 			}
// 			printk("\n");

// 			/* -------------------- FIND "WS," -------------------- */
// 			int ws_start = -1;
// 			for (int i = 0; i < sizeof(rx_buf) - 3; i++)
// 			{
// 				if (rx_buf[i] == 'W' && rx_buf[i+1] == 'S' && rx_buf[i+2] == ',')
// 				{
// 					ws_start = i + 3;   // skip "WS,"
// 					break;
// 				}
// 			}

// 			/* -------------------- FIND WIND SPEED END (',') -------------------- */
// 			int ws_end = ws_start;
// 			while (rx_buf[ws_end] != ',' && rx_buf[ws_end] != '\0')
// 				ws_end++;

// 			/* -------------------- FIND "WD," -------------------- */
// 			int wd_start = -1;
// 			for (int i = 0; i < sizeof(rx_buf) - 3; i++)
// 			{
// 				if (rx_buf[i] == 'W' && rx_buf[i+1] == 'D' && rx_buf[i+2] == ',')
// 				{
// 					wd_start = i + 3;   // skip "WD,"
// 					break;
// 				}
// 			}

// 			/* -------------------- FIND WIND DIRECTION DECIMAL -------------------- */
// 			int dot = wd_start;
// 			while (rx_buf[dot] != '.' && rx_buf[dot] != '\0')
// 				dot++;

// 			/* -------------------- PRINT WIND SPEED -------------------- */
// 			printk("GSM ON - Wind Speed: ");
// 			for (int i = ws_start; i < ws_end; i++)
// 				printk("%c", rx_buf[i]);
// 			printk(" m/s\n");

// 			/* -------------------- PRINT WIND DIRECTION (1 DECIMAL) -------------------- */
// 			printk("GSM ON - Wind Direction: ");
// 			for (int i = wd_start; i < dot; i++)
// 				printk("%c", rx_buf[i]);

// 			// Print one decimal digit
// 			printk(".%c°\n", rx_buf[dot+1]);

// 			k_msleep(5000);
// 		}
		
// 		// Disable GSM for 20 seconds and read values
// 		gsm_disable();
// 		int64_t gsm_off_end = k_uptime_get() + 20000;
		
// 		while (k_uptime_get() < gsm_off_end)
// 		{
// 			// Print raw buffer
// 			for (size_t i = 0; i < sizeof(rx_buf); i++)
// 			{
// 				if (rx_buf[i] != '\0')
// 					printk("%c", rx_buf[i]);
// 				else
// 					printk(" ");
// 			}
// 			printk("\n");

// 			/* -------------------- FIND "WS," -------------------- */
// 			int ws_start = -1;
// 			for (int i = 0; i < sizeof(rx_buf) - 3; i++)
// 			{
// 				if (rx_buf[i] == 'W' && rx_buf[i+1] == 'S' && rx_buf[i+2] == ',')
// 				{
// 					ws_start = i + 3;   // skip "WS,"
// 					break;
// 				}
// 			}

// 			/* -------------------- FIND WIND SPEED END (',') -------------------- */
// 			int ws_end = ws_start;
// 			while (rx_buf[ws_end] != ',' && rx_buf[ws_end] != '\0')
// 				ws_end++;

// 			/* -------------------- FIND "WD," -------------------- */
// 			int wd_start = -1;
// 			for (int i = 0; i < sizeof(rx_buf) - 3; i++)
// 			{
// 				if (rx_buf[i] == 'W' && rx_buf[i+1] == 'D' && rx_buf[i+2] == ',')
// 				{
// 					wd_start = i + 3;   // skip "WD,"
// 					break;
// 				}
// 			}

// 			/* -------------------- FIND WIND DIRECTION DECIMAL -------------------- */
// 			int dot = wd_start;
// 			while (rx_buf[dot] != '.' && rx_buf[dot] != '\0')
// 				dot++;

// 			/* -------------------- PRINT WIND SPEED -------------------- */
// 			printk("GSM OFF - Wind Speed: ");
// 			for (int i = ws_start; i < ws_end; i++)
// 				printk("%c", rx_buf[i]);
// 			printk(" m/s\n");

// 			/* -------------------- PRINT WIND DIRECTION (1 DECIMAL) -------------------- */
// 			printk("GSM OFF - Wind Direction: ");
// 			for (int i = wd_start; i < dot; i++)
// 				printk("%c", rx_buf[i]);

// 			// Print one decimal digit
// 			printk(".%c°\n", rx_buf[dot+1]);

// 			k_msleep(5000);
// 		}
// 	}
// 	// return 0;
// }