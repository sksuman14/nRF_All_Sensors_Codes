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

#define SENSOR_TYPE     0x1E
#define TOTAL_PACKETS    1

/* Global Variables */
struct k_work my_work, acc_work;
struct k_timer my_timer, acc_timer;

/* Advertisement struct */
struct bt_le_ext_adv *adv;

/* Sensor node id */
const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);

/* acc_ptr points to Motion_data[0] and acc_ptr++ points to Motion_data[1] */
uint8_t *acc_ptr;

typedef struct {
    uint8_t x_lsb;
    uint8_t y_lsb;
    uint8_t z_lsb;
} motion_data;

motion_data Motion_data[80];

/* Button variables */
static uint32_t time, last_time; /* used for detecting long button press */
static uint32_t button_counter = 0;
int sleep_flag = 1;
static int Motion_index = 0;
int acc_fetch_flag = 0; /* 0 to 80 indexing */

int err;

/*
 * define the advertising set parameters
 * BT_LE_ADV_OPT_NONE is for non-connectable and non-scannable advertising
 * BT_LE_ADV_OPT_EXT_ADV is for extended advertising
 * BT_LE_ADV_OPT_USE_IDENTITY is for using identity address as the advertiser's address
 * BT_LE_ADV_OPT_NO_2M is used to enable secondary advertising on LE 1M.
 */
struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(
    BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
    0x20,
    0x20,
    NULL); /* NULL because the advertisement is not directed */

/* start advertising with no timeout and exactly one extended advertising event allowed in the extended advertising set. */
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 1); /* number of events is set to 1. */

/* FUNCTION FOR APPROXIMATION */
double round_off(float value)
{
    if (value > 0) {
        if ((int)(value * 10) % 10 <= 4)
            return (uint8_t)(value);
        else
            return (uint8_t)(value + 1);
    } else {
        return (int8_t)(value);
    }
}

void enter_sleep(void)
{
    printk("Entering SYSTEM-OFF mode\n");
    NRF_POWER->SYSTEMOFF = 1;
}

/* function for creating a random static address and setting it as the identity address of the device. */
static void set_random_static_address(void)
{
    int err;
    int err1;
    printk("Starting iBeacon Demo\n");

    bt_addr_le_t addr;

    /* convert the string into a binary address and stores this address in a buffer whose address is addr. */
    err = bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr); /* Only random static address can be given when the type is set to "random" */
    if (err) {
        printk("Invalid BT address (err %d)\n", err);
    }

    /* create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity */
    err1 = bt_id_create(&addr, NULL);
    if (err1 < 0) {
        printk("Creating new ID failed (err %d)\n", err1);
    }
    printk("Created new address\n");
}

/* function to create the extended advertising set using the advertising parameters. */
void adv_param_init(void)
{
    int err;

    err = bt_le_ext_adv_create(&adv_param, NULL, &adv);

    if (err) {
        printk("Failed to create advertising set (err %d)\n", err);
        return;
    }
    printk("Created extended advertising set \n");
}

static int count = 0;

void fetch_acc_data(void)
{
    struct sensor_value accel[3];

    if (sensor == NULL) {
        printf("No device found\n");
        return;
    }
    if (!device_is_ready(sensor)) {
        printf("Device %s is not ready\n", sensor->name);
        return;
    }

    int rc = sensor_sample_fetch(sensor);

    if (rc == 0) {
        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
    }
    if (rc < 0) {
        printf("ERROR: Update failed: %d\n", rc);
    } else {
        /* UPDATE SUCCESSFULL! Sensor works good */
        count++; /* For the counting of (x,y,z) readings as accel[3] */

        printf(" %d ", count);
        printf("x= %f, y= %f, z= %f\n",
               sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));
    }
    /* acc_fetch_flag = 0 now */
    Motion_data[acc_fetch_flag].x_lsb = (int8_t)(round_off(accel[0].val1 * 6.4 + accel[0].val2 * 0.0000064));
    Motion_data[acc_fetch_flag].y_lsb = (int8_t)(round_off(accel[1].val1 * 6.4 + accel[1].val2 * 0.0000064));
    Motion_data[acc_fetch_flag].z_lsb = (int8_t)(round_off(accel[2].val1 * 6.4 + accel[2].val2 * 0.0000064));
    printf("x= %u, y= %u, z=%u\n", Motion_data[acc_fetch_flag].x_lsb, Motion_data[acc_fetch_flag].y_lsb, Motion_data[acc_fetch_flag].z_lsb);

    /* All these values goes in Motion_data which is a struct having 6 values and we will take 41 readings at a time. */
    acc_fetch_flag++; /* flag = 1,2,3,4.....and the motion_data is updated with the new values till flag = 41. */
    /* Hence 6 readings 41 times = 246 bytes. */
    if (acc_fetch_flag == 80) {
        acc_fetch_flag = 0; /* After 41 the next reading will go in Motion_data[0]. */

        k_timer_stop(&acc_timer); /* As it will reach 41st reading --> Then timer is stopped. No further data will be fetched as of now. */
        repeated_adv();
        /* It means that after 10 seconds repeated adv will start. */
    }
}

void start_adv(void)
{
    printk("Start Extended Advertising...");
    err = bt_le_ext_adv_start(adv, &ext_adv_param);

    if (err) {
        printk("Failed to start extended advertising (err %d)\n", err);
        return;
    }
    printk("done.\n");
    k_msleep(100);
    bt_le_ext_adv_stop(adv);
    printk("Stopped advertising..!!\n");

    printk("Entering sleep mode..!!\n");

    Motion_index = 0;
    k_msleep(20000);

    k_timer_start(&acc_timer, K_MSEC(500), K_MSEC(500));
}

int i;
void repeated_adv(void)
{
    int err;

    static uint8_t mfg_data[246];
    static const struct bt_data ad[] = {BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 246)};

    mfg_data[0] = 110; /* Device ID */

    acc_ptr = &Motion_data[Motion_index].x_lsb;

    for (i = 1; i < 241; i++) {
        mfg_data[i] = *(acc_ptr + (i - 1));
        printf("%d ", mfg_data[i]);
    }

    Motion_index = Motion_index + 80;

    printk("Set Extended Advertising Data...\n");
    err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Failed (err %d)\n", err);
        return;
    }
    start_adv();
}

K_WORK_DEFINE(my_work, repeated_adv);

void my_expiry_function(struct k_timer *timer_id)
{
    k_work_submit(&my_work); /* After this submit-> My-work will start executing. */
};
K_TIMER_DEFINE(my_timer, my_expiry_function, NULL);
/* Expiry function is executed each time the timer expires. */

K_WORK_DEFINE(acc_work, fetch_acc_data);
/* Definitions of K_WORK_DEFINE is to initialize the work inside. */
/* Repeated_adv, my_timer and acc_work are defined and initialize. */

/* Hence as soon as the timer is expired --> repeated adv will take place. */
void acc_expiry_function(struct k_timer *timer_id)
{
    k_work_submit(&acc_work);
};
K_TIMER_DEFINE(acc_timer, acc_expiry_function, NULL);

/* Hence as soon as the acc_timer expires --> data will be fetched. */

int j = 0;

void main(void)
{
    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));
    nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led3), gpios)); /* make pin28 high */

    nrf_gpio_cfg_input(DT_GPIO_PIN(DT_NODELABEL(button0), gpios), /* enable button0 as input pin */
                       NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_sense_set(DT_GPIO_PIN(DT_NODELABEL(button0), gpios),
                           NRF_GPIO_PIN_SENSE_LOW); /* Button is 0 when it is pressed. */

    int status = nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)); /* status variable carries current status of the pin. */

    printk("button status=%d\n", status); /* status is 0 when button is pressed, and greater than 0 when button is not pressed */

    while (nrf_gpio_pin_read(DT_GPIO_PIN(DT_NODELABEL(button0), gpios)) == 0) /* return value zero indicates that the button is pressed */
    {
        time = k_uptime_get(); /* get the time elapsed since the device booted up (in milliseconds) */

        if (button_counter == 0) {
            last_time = time; /* will be executed only once. */
        }

        if (last_time + 5000 == time) /* counts 5 seconds since the start of the button press. Change the value to 2000 for 2 seconds long press */
        {
            button_counter = 0;
            sleep_flag = 0; /* the flag takes the value zero only when the button is pressed continuously for 5 seconds */
            printk("Button was pressed continuously for 5 seconds..\n");
            printk("last time=%d\n", last_time);
            printk("time=%d\n", time);

            while (j <= 5) /* toggling of led0 is used as an idication that 5 seconds have elapsed since we press the button */
            {
                nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); /* configure led0 as the output pin */
                nrf_gpio_pin_toggle(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); /* toggle the LED */
                k_sleep(K_MSEC(50));
                j++;
            }
            break; /* when button is pressed continuously for 5 seconds, blink led0. Then come out of the loop, set parameters and start advertising */
        }

        button_counter = button_counter + 1;
    }

    while (sleep_flag) {
        enter_sleep(); /* as long as the sleep flag is 1, stay in deep sleep mode. */
    }

    nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); /* switch off the LED */
    int err;

    printk("Starting Periodic Advertising Demo\n");

    /* Create a random static address and set it as the identity address of the device */
    set_random_static_address();

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    adv_param_init();
    k_timer_start(&acc_timer, K_MSEC(500), K_MSEC(500));
}