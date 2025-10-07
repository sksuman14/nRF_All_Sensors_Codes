/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/sensor.h>

struct bt_le_ext_adv *adv;

int err;
/* Button variables */
static uint32_t time, last_time; // used for detecting long button press
static uint32_t button_counter = 0;
int button_tick = 0;
int sleep_flag = 1;
int counter= 5;

// define the advertising set parameters
// BT_LE_ADV_OPT_NONE is for non-connectable and non-scannable advertising
// BT_LE_ADV_OPT_EXT_ADV is for extended advertising
// BT_LE_ADV_OPT_USE_IDENTITY is for using identity address as the advertiser's address
// BT_LE_ADV_OPT_NO_2M is used to enable secondary advertising on LE 1M.

struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
                                                        0x30,
                                                        0x30, // advertising interval is set to 3 seconds
                                                        NULL);
// start advertising with no timeout and exactly one extended advertising event allowed in the extended advertising set.
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 1); // number of events is set to 2.
static uint8_t mfg_data[8] = {0};
static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 8),
};

// function for creating a random static address and setting it as the identity address of the device.
static void set_random_static_address(void)
{

        printk("Starting iBeacon Demo\n");

        bt_addr_le_t addr;

        // convert the string into a binary address and stores this address in a buffer whose address is addr.
        err = bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr); // Only random static address can be given when the type is set to "random"
        if (err)
        {
                printk("Invalid BT address (err %d)\n", err);
        }

        // create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity
        err = bt_id_create(&addr, NULL);
        if (err < 0)
        {
                printk("Creating new ID failed (err %d)\n", err);
        }
        printk("Created new address\n");
}

// function to create the extended advertising set using the advertising parameters.
void adv_param_init(void)
{
        int err;

        err = bt_le_ext_adv_create(&adv_param, NULL, &adv);

        if (err)
        {
                printk("Failed to create advertising set (err %d)\n", err);
                return;
        }
        printk("Created extended advertising set \n");
}
void turnoff()
{
        int j=0;
        sleep_flag = 0;
        enter_sleep();
        nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));
        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led3), gpios)); // make pin28 high

        nrf_gpio_cfg_input(DT_GPIO_PIN(DT_NODELABEL(button0), gpios), // enable button0 as input pin
                           NRF_GPIO_PIN_PULLUP);
        nrf_gpio_cfg_sense_set(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
                               NRF_GPIO_PIN_SENSE_LOW); // Button is 0 when it is pressed.

        int status = nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)); // status variable carries current status of the pin.

        if (nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)) == 0)
        {
                 while (j <= 5) // toggling of led0 is used as an idication that 5 seconds have elapsed since we press the button
                        {
                                nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led1), gpios)); // configure led0 as the output pin
                                nrf_gpio_pin_toggle(DT_GPIO_PIN(DT_NODELABEL(led1), gpios)); // toggle the LED
                                k_sleep(K_MSEC(50));
                                j++;
                        }
               
                nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led1), gpios)); // switch off the LED

                // enter_sleep();
        }
        
}

void start_adv(void)
{

        /* Start advertising */
        if (counter)
        {
               counter = counter - 1 ;
        }
        else
        {
                turnoff();
        }

        err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
        if (err)
        {
                printk("Failed (err %d)\n", err);
                return;
        }
        printk("Start Extended Advertising...");
        err = bt_le_ext_adv_start(adv, &ext_adv_param); // BT_LE_EXT_ADV_START_DEFAULT);

        if (err)
        {
                printk("Failed to start extended advertising "
                       "(err %d)\n",
                       err);
                return;
        }
        printk("done.\n");

        // enter sleep mode after advertising
        k_msleep(500);

        bt_le_ext_adv_stop(adv);
        printk("Stopped advertising..!!\n");
        start_adv();
}

void enter_sleep(void)
{

        printk("Entering SYSTEM-OFF mode\n");
        NRF_POWER->SYSTEMOFF = 1;
}

int j = 0;
void main()
{
        // /////////////////////////////////////////////////////////

        nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));
        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led3), gpios)); // make pin28 high

        nrf_gpio_cfg_input(DT_GPIO_PIN(DT_NODELABEL(button0), gpios), // enable button0 as input pin
                           NRF_GPIO_PIN_PULLUP);
        nrf_gpio_cfg_sense_set(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
                               NRF_GPIO_PIN_SENSE_LOW); // Button is 0 when it is pressed.

        int status = nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)); // status variable carries current status of the pin.

        printk("button status=%d\n", status); // status is 0 when button is pressed, and greater than 0 when button is not pressed

        while (nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)) == 0) // return value zero indicates that the button is pressed
        {

                time = k_uptime_get(); // get the time elapsed since the device booted up (in milliseconds)

                if (button_counter == 0)
                {
                        last_time = time; // will be executed only once.
                }

                if (last_time + 500 == time) // counts 5 seconds since the start of the button press. Change the value to 2000 for 2 seconds long press
                {

                        button_counter = 0;
                        sleep_flag = 0; // the flag takes the value zero only when the button is pressed continuously for 5 seconds
                        printk("Button was pressed continuously for 5 seconds..\n");
                        printk("last time=%d\n", last_time);
                        printk("time=%d\n", time);

                        while (j <= 5) // toggling of led0 is used as an idication that 5 seconds have elapsed since we press the button
                        {
                                nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // configure led0 as the output pin
                                nrf_gpio_pin_toggle(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // toggle the LED
                                k_sleep(K_MSEC(50));
                                j++;
                        }
                        break; // when button is pressed continuously for 5 seconds, blink led0. Then come out of the loop, set parameters and start advertising
                }

                button_counter = button_counter + 1;
        }

        while (sleep_flag)
        {
                enter_sleep(); // as long as the sleep flag is 1, stay in deep sleep mode.
        }

        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // switch off the LED
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        mfg_data[0] = 1; // device id

        /* Create a random static address and set it as the identity address of the device */
        set_random_static_address();

        /* Initialize the Bluetooth Subsystem */
        err = bt_enable(NULL);
        if (err)
        {
                printk("Bluetooth init failed (err %d)\n", err);
                return;
        }
        adv_param_init();
        start_adv();
}