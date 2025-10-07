#include <zephyr/zephyr.h>
#include <zephyr/kernel.h> // header files for the project
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/bluetooth/controller.h>

struct bt_le_ext_adv *adv;                                           // Declare a pointer to Bluetooth Low Energy extended advertisement structure
const struct device *const sht = DEVICE_DT_GET_ANY(sensirion_sht4x); // Retrieve the SHT device from the device tree

int8_t temp_value1, temp_value2; // Declare variables to store temperature and humidity values

int8_t hum_value1, hum_value2;

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

int err; // Declare a variable to store error codes

struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
                                                        0x30,
                                                        0x30, // advertising interval is set to 3 seconds
                                                        NULL);
// start advertising with no timeout and exactly one extended advertising event allowed in the extended advertising set.
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 2); // number of events is set to 2.
static uint8_t mfg_data[8] = {0};                                                      // Define a static array to store manufacturer data with a size of 8 bytes
static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 8), // Define Bluetooth data with manufacturer-specific data
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

// function for creating a random static address and setting it as the identity address of the device.
static void set_random_static_address(void) // Function to set a random static address
{

  printk("Starting iBeacon Demo\n"); // Print a message indicating the start of the iBeacon demo

  bt_addr_le_t addr; // Declare a Bluetooth Low Energy address structure

  // convert the string into a binary address and stores this address in a buffer whose address is addr.
  err = bt_addr_le_from_str("FE:BC:BE:AF:BA:11", "random", &addr); // Only random static address can be given when the type is set to "random"
  if (err)                                                         // Check if there was an error in setting the Bluetooth address
  {
    printk("Invalid BT address (err %d)\n", err); // Print an error message indicating the failure to set the Bluetooth address
  }

  // create a new Identity address. This must be done before enabling Bluetooth. Addr is the address used for the new identity
  err = bt_id_create(&addr, NULL); // Attempt to create a new Bluetooth identity using the provided address
  if (err < 0)                     // Check if there was an error in creating the new Bluetooth identity
  {
    printk("Creating new ID failed (err %d)\n", err); // Print an error message indicating the failure to create a new identity
  }
  printk("Created new address\n"); // Print a message indicating the successful creation of a new address
}

// function to create the extended advertising set using the advertising parameters.
void adv_param_init(void) // Function to initialize advertising parameters
{
  int err;

  err = bt_le_ext_adv_create(&adv_param, NULL, &adv); // Attempt to create an extended advertising set

  if (err) // Check if there was an error in creating the advertising set
  {
    printk("Failed to create advertising set (err %d)\n", err); // Print an error message indicating the failure to create the advertising set
    return;
  }
  printk("Created extended advertising set \n"); // Print a message indicating the successful creation of the extended advertising set
}

void fetch_temp_data(void) // Function to fetch temperature data

{

  // Set manufacturer data for device identification

  mfg_data[0] = 0x59; // Data type: Complete list of 16-bit UUIDs available
  mfg_data[1] = 0x00; // Length of the data following this field (including this field)
  mfg_data[2] = 172;  // device id

  // currently looping only once since i=1
  for (int i = 0; i < 1; i++) // Loop to fetch temperature and humidity data multiple times
  {
    struct sensor_value temp, hum; // Declare variables to store temperature and humidity values

    err = sensor_sample_fetch(sht); // Fetch sensor samples from the SHT device

    if (err == 0) // Check if the sampling operation was successful
    {
      sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp); // fetch the temperature reading and store it in temp

      sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum);                                                    // Get humidity data from the sensor
      printf("Temperature= %f; Humidity= %f\n", sensor_value_to_double(&temp), sensor_value_to_double(&hum)); // Print temperature and humidity values
    }
    else
    {
      printf("ERROR: Temperature data update failed: %d\n", err); // Print an error message indicating the failure to update temperature data
    }

    temp_value1 = temp.val1;         // this variable stores only the value before the decimal
    temp_value2 = temp.val2 / 10000; // this variable stores only the value after the decimal

    hum_value1 = hum.val1;         // this variable stores only the value before the decimal
    hum_value2 = hum.val2 / 10000; // this variable stores only the value after the decimal

    // put the logged sensor data into data buffers

    mfg_data[i * 4 + 3] = temp_value1; // Store the first byte of temperature value
    mfg_data[i * 4 + 4] = temp_value2; // Store the second byte of temperature value
    mfg_data[i * 4 + 5] = hum_value1;  // Store the first byte of humidity value
    mfg_data[i * 4 + 6] = hum_value2;  // Store the second byte of humidity value

    // k_msleep(3000);
  }

  mfg_data[7] = 0x00;
  start_adv(); // Function to start advertising
}

void start_adv(void)
{

  /* Start advertising */

  err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0); // Set advertising data for the extended advertising set
  if (err)                                                        // Check if there was an error in setting the advertising data
  {
    printk("Failed (err %d)\n", err); // Print an error message indicating the failure to set the advertising data
    return;
  }
  printk("Start Extended Advertising...");        // Print a message indicating the start of extended advertising
  err = bt_le_ext_adv_start(adv, &ext_adv_param); // BT_LE_EXT_ADV_START_DEFAULT);

  if (err) // Start extended advertising with the specified parameters
  {
    printk("Failed to start extended advertising " // Check if there was an error in starting extended advertising
           "(err %d)\n",
           err);
    return;
  }
  printk("done.\n"); // Print a message indicating the completion of extended advertising start

  // enter sleep mode after advertising
  k_msleep(100); // Delay execution for 100 milliseconds

  bt_le_ext_adv_stop(adv);             // Stop extended advertising
  printk("Stopped advertising..!!\n"); // Print a message indicating the cessation of advertising
  fetch_temp_data();                   // Fetch temperature data
}

void main(void) // main function
{
  nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led3), gpios)); // pin 27 aka VDD HIGH FOR SHT-40. CHECK DTS AND OVERLAY ALWAYS.
  nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led3), gpios));    // make pin27 high
  printk("Starting Temperature and Humidity Node\n");          // Print a message indicating the start of the Temperature and Humidity Node

  /* Create a random static address and set it as the identity address of the device */
  set_random_static_address();

  /* Initialize the Bluetooth Subsystem */
  err = bt_enable(NULL); // Enable Bluetooth stack initialization
  if (err)               // Check if there was an error in initializing the Bluetooth stack
  {
    printk("Bluetooth init failed (err %d)\n", err); // Print an error message indicating the failure to initialize Bluetooth
    return;
  }
  adv_param_init();  // Initialize advertising parameters
  fetch_temp_data(); // Fetch temperature data
}
