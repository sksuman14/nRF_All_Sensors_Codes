
#include <stddef.h>

#include "headers/main_observer.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/pm/pm.h>
#include <zephyr/device.h>
#include <hal/nrf_gpio.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DELAY_TIME K_MSEC(1000)

int ret;
/* The devicetree node identifier for the "led0" alias. */

#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);


#define STACKSIZE	 2048
#define THREAD0_PRIORITY 7

static int connection_flag = 0;
static int counter_remote1 = 0;
// static int counter_remote2 = 0;
LOG_MODULE_DECLARE(gateway, LOG_LEVEL_DBG);

/* Index write_index to write data in flash */
// extern int write_index = 0;
#define n  8
#define n1 8

/* It is data callback function. It is executed whenever the system recieves an advertising packet.
 */
#if defined(CONFIG_BT_EXT_ADV)
static bool data_cb(struct bt_data *data, void *user_data)
{

	int total_write = pow(
		2, n); // total number of bytes to be written corresponding to one received packet
	int chunk_size = pow(
		2, n1); // total number of bytes to be written in one write operation [max.= 128]
	// int num_steps = pow(2, (n - n1)); // number of iterations in which the entire packet gets
	// written into the memory
	uint8_t write_array[chunk_size];
	write_array[0] = 0;

	/* data_cb is executed whenever some advertising packet comes. */
	LOG_INF("IN DATA CB");

	// len is the length of the advertising packet.
	// uint8_t len;

	switch (data->type) {
	case BT_DATA_NAME_SHORTENED:
	case BT_DATA_NAME_COMPLETE:
	case BT_DATA_MANUFACTURER_DATA:

		int index = 0;

		/* It is just a variable that runs from 0 to 128. */
		int j = 0;

		// Storing chunk_size BYTES OF DATA AT ONCE //
		for (j = 0; j < chunk_size; j++) {
			/* Using char (char = 1 byte uint8_t type). */
			char number = data->data[j + index];
			/* Appending them in write array. */
			write_array[j] = number;
			printk("%d ", write_array[j]);
		}
	}
}

static const char *phy2str(uint8_t phy)
{
	switch (phy) {
	case BT_GAP_LE_PHY_NONE:
		return "No packets";
	case BT_GAP_LE_PHY_1M:
		return "LE 1M";
	case BT_GAP_LE_PHY_2M:
		return "LE 2M";
	case BT_GAP_LE_PHY_CODED:
		return "LE Coded";
	default:
		return "Unknown";
	}
}
static void scan_recv(const struct bt_le_scan_recv_info *info, struct net_buf_simple *buf)
{
	/* Executed when we recieve the packet from advertising node. */
	char le_addr[BT_ADDR_LE_STR_LEN];
	uint8_t data_status;
	uint16_t data_len;
	char name[30];
	//  (void)memset(name, 0, sizeof(name));

	data_len = buf->len;
	bt_data_parse(buf, data_cb, name);

	data_status = BT_HCI_LE_ADV_EVT_TYPE_DATA_STATUS(info->adv_props);

	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));
}

static struct bt_le_scan_cb scan_callbacks = {
	.recv = scan_recv,
};
#endif /* CONFIG_BT_EXT_ADV */

int observer_start(void)
{

	bt_addr_le_t addr1;

	bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr1);

	int err = bt_le_filter_accept_list_add(&addr1);
	if (err == 0) {
		printk("Successfully added the address to Filter Accept List..\n");
	} else {
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

	if (err) {
		LOG_INF("Start scanning failed (err %d)\n", err);
		return err;
	}
	LOG_INF("Started scanning...\n");
	return 0;
}

int init_observer(void)
{
	/* Observer Initialization. */
	LOG_ERR("observer init.....");

	/* Enabling Bluetooth. */
	int err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return err;
	}

	return 0;
}

