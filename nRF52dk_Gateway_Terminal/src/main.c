/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>



int observer_start(void);


static void set_random_static_address(void)
{
int err;
        int err1;
	printk("Starting iBeacon Demo\n");
        
		bt_addr_le_t addr;

		//convert the string into a binary address and stores this address in a buffer whose address is addr. 
		err=bt_addr_le_from_str("DE:AD:BE:AF:BA:11","random" , &addr);  //Only random static address can be given when the type is set to "random"
                if (err) {
			printk("Invalid BT address (err %d)\n", err);
						}

                //create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity
		err1=bt_id_create(&addr, NULL);
                if (err1 < 0) {
			printk("Creating new ID failed (err %d)\n", err1);
		}
            printk("Created new address\n");
 }

void main(void)
{
	int err;

	printk("Starting Observer Demo\n");

	set_random_static_address();

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	(void)observer_start();

	// printk("Exiting %s thread.\n", __func__);
}