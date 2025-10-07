#include <zephyr/zephyr.h>
#include <zephyr/kernel.h>                                   // header files for the project
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/bluetooth/controller.h>
#include <hal/nrf_gpio.h>

#define SENSOR_TYPE     0x1E; 
#define TOTAL_PACKETS    1;

struct bt_le_ext_adv *adv;                                                            // Declare a pointer to Bluetooth Low Energy extended advertisement structure
const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);             

int8_t accel_value_xa, accel_value_xb;                                                    // Declare a 8 bit signed integer variables to accelorometer values
int8_t accel_value_ya, accel_value_yb;
int8_t accel_value_za, accel_value_zb;

int err;                                                                           // Declare a variable to store error codes

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)                     // It includes the null-terminator (\0) at the end of the string, Subtracting 1 gives the actual length of the string 

struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M, 0x30, 0x30, NULL);
// start advertising with no timeout and exactly one extended advertising event allowed in the extended advertising set.
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 2); // number of events is set to 2.
static uint8_t mfg_data[10] = {0};              // Define a static array to store manufacturer-specific data included in the Bluetooth advertising packets with a size of 10 bytes
static const struct bt_data ad[] = {
      BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 10),       // Define Bluetooth data with manufacturer-specific data
	    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)               // Define Bluetooth data with the complete device name  
                                             
  };
  

//function for creating a random static address and setting it as the identity address of the device.
static void set_random_static_address(void)                     // Function to set a random static address
{
  
  printk("Starting iBeacon Demo\n");                           // Print a message indicating the start of the iBeacon demo

  bt_addr_le_t addr;       //This declares a variable named 'addr' of type 'bt_addr_le_t'. This structure is used to hold a Bluetooth Low Energy (LE) address.     

  // convert the string into a binary address and stores this address in a buffer whose address is addr.
  err = bt_addr_le_from_str("DE:AD:BE:AF:BA:11", "random", &addr); // The second parameter, "random", specifies that the address is a random static address.
  if (err)                                                          // err will hold the return value of this function. If the conversion is successful, err will be 0. 
  {
    printk("Invalid BT address (err %d)\n", err);                    // Print an error message indicating the failure to set the Bluetooth address
  }

  // create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity
  err = bt_id_create(&addr, NULL);                                    // Attempt to create a new Bluetooth identity using the provided address
  if (err < 0)                                                             // Check if there was an error in creating the new Bluetooth identity
  {
    printk("Creating new ID failed (err %d)\n", err);                        
  }
  printk("Created new address\n");                                          
}

// function to create the extended advertising set using the advertising parameters.
void adv_param_init(void)                                         // Function to initialize advertising parameters
{
  int err;                                                        
 
  err = bt_le_ext_adv_create(&adv_param, NULL, &adv);           // Attempt to create an extended advertising set, 'NULL' indicating that there are no additional parameters or options provided. 

  if (err)                                                   // Check if there was an error in creating the advertising set
  {
    printk("Failed to create advertising set (err %d)\n", err);       // Print an error message indicating the failure to create the advertising set
    return;                                                           
  }
  printk("Created extended advertising set \n");                       
}

void fetch_acc_data(void)
{
    // Initialize GPIO pins for LEDs
    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));    //configures a specified GPIO pin as an output
    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));    //The macro 'DT_GPIO_PIN' is used to get the GPIO pin number from the device tree
    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led2), gpios));

    // // Initially turn off all LEDs (Active low logic)
    // nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
    // nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));
    // nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led2), gpios));
    
    mfg_data[0] = 0x59;
    mfg_data[1] = 0x00;
    mfg_data[2] = 158; //device id

        struct sensor_value accel[3];
        
        err = sensor_sample_fetch(sensor); // Fetch sensor samples from the sensor device

        if (err == 0) // Check if the sampling operation was successful
        {
            sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);                 //retrieves the X, Y, and Z values from the sensor and stores them in the 'accel' array
            printf("x= %f, y= %f, z= %f\n", sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));
        }
        else
        {
            printf("ERROR: Accelerometer data update failed: %d\n", err);
        }

        accel_value_xb = accel[0].val1; // X value before the decimal
        accel_value_xa = accel[0].val2 / 10000; // X value after the decimal
        accel_value_yb = accel[1].val1; // Y value before the decimal
        accel_value_ya = accel[1].val2 / 10000; // Y value after the decimal
        accel_value_zb = accel[2].val1; // Z value before the decimal
        accel_value_za = accel[2].val2 / 10000; // Z value after the decimal

        if (accel_value_xb == 0 && accel_value_yb == 0 && (accel_value_zb >=9 || accel_value_zb <=-9 ))
        {
            nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led2), gpios)); //clear indiactes led 'ON' and set indicates led 'OFF'
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));
        }
        else if (accel_value_yb == 0 && accel_value_zb == 0 && (accel_value_xb >=9 || accel_value_xb <=-9 ))
        {
            nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led2), gpios));
        }
        else if (accel_value_xb == 0 && accel_value_zb == 0 && (accel_value_yb >=9 || accel_value_yb <=-9 ))                                                                                                                                        
        {
            nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led2), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
        }

        else{
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led1), gpios));
            nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led2), gpios));
        }

        // Put the logged sensor data into data buffers
        mfg_data[3] = accel_value_xb; // First byte of X value
        mfg_data[4] = accel_value_xa; // Second byte of X value
        mfg_data[5] = accel_value_yb; // First byte of Y value
        mfg_data[6] = accel_value_ya; // Second byte of Y value
        mfg_data[7] = accel_value_zb; // First byte of Z value
        mfg_data[8] = accel_value_za; // Second byte of Z value

    mfg_data[9] = 0x00;
    start_adv(); // Function to start advertising
}

void start_adv(void)            
{
  /* Start advertising */

  err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);                       // Setting the advertising data or scan response data for extended advertising set
  if (err)                                                                              // Check if there was an error in setting the advertising data
  {
    printk("Failed (err %d)\n", err);                                                    // Print an error message indicating the failure to set the advertising data
    return;
  }
  printk("Start Extended Advertising...");                                                     // Print a message indicating the start of extended advertising

  err = bt_le_ext_adv_start(adv, &ext_adv_param);                                               // BT_LE_EXT_ADV_START_DEFAULT
  if (err)                                                                                   // Start extended advertising with the specified parameters
  {
    printk("Failed to start extended advertising" "(err %d)\n",  err);                                       // Check if there was an error in starting extended advertising
    return;
  }
  printk("done.\n");                                                      // Print a message indicating the completion of extended advertising start

  // enter sleep mode after advertising
  k_msleep(50);                                           // Delay execution for 100 milliseconds
   
  bt_le_ext_adv_stop(adv);                               // Stop extended advertising
  printk("Stopped advertising..!!\n");                   // Print a message indicating the cessation of advertising
  fetch_acc_data();                                     // Fetch temperature data

}

void main(void)                             // main function
{
  // nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));    //pin 27 aka VDD HIGH FOR SHT-40. CHECK DTS AND OVERLAY ALWAYS.
  // nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));       // make pin27 high
  printk("Starting Accelorometer Node\n");         

  /* Create a random static address and set it as the identity address of the device */
  set_random_static_address();

  /* Initialize the Bluetooth Subsystem */
  err = bt_enable(NULL);                                               // Enable Bluetooth stack initialization
  if (err)                                                            // Check if there was an error in initializing the Bluetooth stack
  {
    printk("Bluetooth init failed (err %d)\n", err);                 // Print an error message indicating the failure to initialize Bluetooth
    return;
  }
  adv_param_init();                                                  // Initialize advertising parameters
  fetch_acc_data();       
}
