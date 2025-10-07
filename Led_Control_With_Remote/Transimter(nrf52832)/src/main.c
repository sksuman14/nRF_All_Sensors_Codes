
#include <zephyr/bluetooth/bluetooth.h>              // header files for the project
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/sensor.h>

struct bt_le_ext_adv *adv;   // Declaration of a pointer variable named 'adv'

int err; 
/* Button variables */
static uint32_t time, last_time; // used for detecting long button press
static uint32_t button_counter = 0;  // Static variable 'button_counter' of type uint32_t (unsigned 32-bit integer) initialized to 0
int button_tick = 0;  // Integer variable 'button_tick' initialized to 0
int sleep_flag = 1;   // Integer variable 'button_tick' initialized to 0
int counter= 1;        // Integer variable 'counter' initialized to 1

struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
                                                        0x30,
                                                        0x30, // advertising interval is set to 3 seconds
                                                        NULL);
// start advertising with no timeout and exactly one extended advertising event allowed in the extended advertising set.
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 1); // number of events is set to 2.
static uint8_t mfg_data[8] = {0};   // Static array 'mfg_data' of type uint8_t (unsigned 8-bit integer) with 8 elements, initialized to 0
static const struct bt_data ad[] = {   
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 8),      // Static constant array 'ad' of struct bt_data type, containing a single element
};

// function for creating a random static address and setting it as the identity address of the device.
static void set_random_static_address(void)  // Print a message indicating the start of the iBeacon Demo
{

        printk("Starting iBeacon Demo\n");   // Print a message indicating the start of the iBeacon Demo

        bt_addr_le_t addr;  // Declaration of a Bluetooth Low Energy address structure variable named 'addr'

        // convert the string into a binary address and stores this address in a buffer whose address is addr.
        err = bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr); // Only random static address can be given when the type is set to "random"
        if (err)
        {
                printk("Invalid BT address (err %d)\n", err);  // Print an error message with the error code
        }

        // create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity
        err = bt_id_create(&addr, NULL); // Create a new Bluetooth ID using the provided address (addr) and no additional parameters

        if (err < 0)   /* Check if error occurred during initialization */
        {
                printk("Creating new ID failed (err %d)\n", err); // Print an error message if creating the new ID failed
        }
        printk("Created new address\n");   // Print a message indicating that a new address has been successfully created
}

// function to create the extended advertising set using the advertising parameters.
void adv_param_init(void)
{
        int err;  // Declaration of the error variable to store return codes or error status
    

        err = bt_le_ext_adv_create(&adv_param, NULL, &adv);  // Create a new extended advertising instance and store the return code in 'err'

        if (err)  // Check if an error occurred during extended advertising creation
        {
                printk("Failed to create advertising set (err %d)\n", err);   // Print an error message indicating failed advertising set creation with the specific error code
                return;  // Exit the function early due to the failed advertising set creation
        }
        printk("Created extended advertising set \n");  // Print a message indicating successful creation of an extended advertising set
}
void turnoff()
{ 
        int j=0; // Declaration and initialization of integer variable 'j' to 0
        sleep_flag = 0;  // Set the sleep_flag variable to 0, indicating that sleep mode is disabled
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
               counter = counter - 1 ; // decrement the counter by 1
        }
        else
        {
                turnoff();
        }

        err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);  // Set extended advertising data for the advertising instance 'adv' using the advertisement data 'ad' array
        if (err)   /* Check if an error occurred while setting extended advertising data */
        {
                printk("Failed (err %d)\n", err);  // Print an error message indicating the failure and include the error code in the message
                return;   // Exit the current function due to the failure in the previous operation
        }
        printk("Start Extended Advertising...");  // Print a message indicating the start of extended advertising
        err = bt_le_ext_adv_start(adv, &ext_adv_param); // BT_LE_EXT_ADV_START_DEFAULT);

        if (err)  /* Check if an error occurred while setting extended advertising data */
        {
                printk("Failed to start extended advertising "  // Print a message indicating the failing of the start extended advertising
                       "(err %d)\n",
                       err);
                return;   // Exit the current function due to the failure in the previous operation
        }
        printk("done.\n"); 

        // enter sleep mode after advertising
        k_msleep(500);  // delay time 

        bt_le_ext_adv_stop(adv);   // Stop extended advertising for the advertising instance 'adv'
        printk("Stopped advertising..!!\n");   // Print a message indicating that advertising has been stopped
        start_adv();   // Call a function named 'start_adv' to initiate advertising again
}

void enter_sleep(void)
{

        printk("Entering SYSTEM-OFF mode\n");  // Print a message indicating that the system is entering SYSTEM-OFF mode
        NRF_POWER->SYSTEMOFF = 1;  // Set the SYSTEMOFF bit in the NRF_POWER register to enter SYSTEM-OFF mode
}

int j = 0;
void main()   // main function
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
                        printk("Button was pressed continuously for 5 seconds..\n");  // Print a message indicating that the button was pressed continuously for 5 seconds
                        printk("last time=%d\n", last_time);  // Print the value of 'last_time' variable
                        printk("time=%d\n", time);  // Print the value of 'time' variable

                        while (j <= 5) // toggling of led0 is used as an idication that 5 seconds have elapsed since we press the button
                        {
                                nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // configure led0 as the output pin
                                nrf_gpio_pin_toggle(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // toggle the LED
                                k_sleep(K_MSEC(50));   // delay time
                                j++; // increment the j by 1
                        }
                        break; // when button is pressed continuously for 5 seconds, blink led0. Then come out of the loop, set parameters and start advertising
                }

                button_counter = button_counter + 1;        // increment the button counter by 1
        }

        while (sleep_flag)   // infinite loop
        {
                enter_sleep(); // as long as the sleep flag is 1, stay in deep sleep mode.
        }

        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // switch off the LED
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        mfg_data[0] = 1; // device id

        /* Create a random static address and set it as the identity address of the device */
        set_random_static_address();

        /* Initialize the Bluetooth Subsystem */
        err = bt_enable(NULL);    // Enable Bluetooth functionality with default parameters
        if (err)
        {
                printk("Bluetooth init failed (err %d)\n", err);   // print a  message indicating that bluetooth initialization is failed
                return;  
        }
        adv_param_init();  // Initialize advertising parameters
        start_adv();   // Start advertising
}
