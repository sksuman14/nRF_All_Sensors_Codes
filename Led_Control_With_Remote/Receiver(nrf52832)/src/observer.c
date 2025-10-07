#include <stddef.h>
#include "headers/main_observer.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/sys/printk.h>
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <math.h>

#include <errno.h>
#include <string.h>
#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>

#define DELAY_TIME K_MSEC(1000)

// Define remote devices
uint8_t remote1 = 1;
uint8_t remote2 = 2;

/* GPIO pin definitions for LEDs on the board */
#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

#define STACKSIZE 2048   // Stack size for threads
#define THREAD0_PRIORITY 7  // Thread priority

// Variables to manage remote counters and connection state
static int connection_flag = 0;
static int counter_remote1 = 0;
static int counter_remote2 = 0;
LOG_MODULE_DECLARE(gateway, LOG_LEVEL_DBG);  // Declare log module for the gateway

/* It is data callback function. It is executed whenever the system receives an advertising packet. */
#if defined(CONFIG_BT_EXT_ADV)
static bool data_cb(struct bt_data *data, void *user_data)
{
	/* data_cb is executed whenever some advertising packet comes. */
	LOG_INF("IN DATA CB");  // Log message for entering data callback

	// Check the type of Bluetooth advertising data
	switch (data->type)
	{
	case BT_DATA_NAME_SHORTENED:
	case BT_DATA_NAME_COMPLETE:
	case BT_DATA_MANUFACTURER_DATA:

		int index = 0;
		uint8_t write_array[3]; // Store received data (max 3 bytes)
		write_array[0] = 0;

		/* Copy data into write_array */
		for (int j = 0; j < 3; j++)
		{
			char number = data->data[j + index];
			write_array[j] = number;
			printk("%d ", write_array[j]); // Print the received byte
		}

		// Handle remote 1 (example remote device with identifier 1)
		if (write_array[0] == remote1)
		{
			counter_remote1++;
			if (counter_remote1 == 1)
			{
				// Turn on the board LED
				gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
				gpio_pin_set_dt(&led0, 1); // Set LED to ON
			}
			else if (counter_remote1 == 2)
			{
				// Turn off the board LED
				gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
				gpio_pin_set_dt(&led0, 0); // Set LED to OFF
				counter_remote1 = 0;
			}
		}
		// Handle remote 2 (example remote device with identifier 2)
		if (write_array[0] == remote2)
		{
			counter_remote2++;
			if (counter_remote2 == 1)
			{
				// Turn on the board LED
				gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
				gpio_pin_set_dt(&led0, 1); // Set LED to ON
			}
			else if (counter_remote2 == 2)
			{
				// Turn off the board LED
				gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
				gpio_pin_set_dt(&led0, 0); // Set LED to OFF
				counter_remote2 = 0;
			}
		}
	}
	return true;
}
#endif /* CONFIG_BT_EXT_ADV */

// Scan callback function for handling received BLE advertising packets
static void scan_recv(const struct bt_le_scan_recv_info *info, struct net_buf_simple *buf)
{
	/* Executed when we receive the packet from the advertising node. */
	char le_addr[BT_ADDR_LE_STR_LEN];
	uint8_t data_status;
	uint16_t data_len;
	char name[30];   // Name of the device (advertising data)

	data_len = buf->len;
	bt_data_parse(buf, data_cb, name); // Parse the advertisement data and call data_cb

	data_status = BT_HCI_LE_ADV_EVT_TYPE_DATA_STATUS(info->adv_props);
	// Convert the Bluetooth address to string and print it
	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));
}

static struct bt_le_scan_cb scan_callbacks = {
	.recv = scan_recv,
};

int observer_start(void)
{
	bt_addr_le_t addr1;

	bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr1); // Convert MAC address to bt_addr_le_t structure

	int err = bt_le_filter_accept_list_add(&addr1);  // Add address to the filter accept list
	if (err == 0)
	{
		printk("Successfully added the address to Filter Accept List..\n");
	}
	else
	{
		printk("Failed to update Filter Accept List..\n");
	}

	struct bt_le_scan_param scan_param = {
		.type = BT_LE_SCAN_TYPE_PASSIVE,
		.options = BT_LE_SCAN_OPT_FILTER_DUPLICATE | BT_LE_SCAN_OPT_FILTER_ACCEPT_LIST,
		.interval = 0x0030,
		.window = 0x0030,
	};

	bt_le_scan_cb_register(&scan_callbacks);
	err = bt_le_scan_start(&scan_param, NULL);

	if (err)
	{
		LOG_INF("Start scanning failed (err %d)\n", err);
		return err;
	}
	LOG_INF("Started scanning...\n");
	return 0;
}

// Initialize the observer (Bluetooth setup and configuration)
int init_observer(void)
{
	/* Observer Initialization. */
	LOG_ERR("observer init.....");

	/* Enabling Bluetooth. */
	int err = bt_enable(NULL);
	if (err)
	{
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return err;
	}

	return 0;
}
