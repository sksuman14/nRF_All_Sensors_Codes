#include <zephyr/zephyr.h>              // Zephyr kernel API
#include <zephyr/bluetooth/bluetooth.h> // Bluetooth API
#include <zephyr/drivers/gpio.h>        // GPIO API
#include <zephyr/device.h>              // Device API
#include <zephyr/pm/pm.h>               // Power management API
#include <hal/nrf_gpio.h>               // nRF GPIO API
#include <zephyr/drivers/sensor.h>      // Sensor API

// Global definitions for flexibility
#define TEMP_FETCH_INTERVAL_MS 3000     // Sensor fetch interval in ms
#define ADVERTISING_STOP_DELAY_MS 500  // Delay to stop advertising in ms
#define NUM_SENSOR_READINGS 10         // Number of sensor readings
#define MFG_DATA_SIZE 64               // Size of manufacturer data buffer
#define BT_ADDRESS "DE:AD:BB:AA:BB:11" // Random static BT address

struct bt_le_ext_adv *adv;                                           // BLE extended advertisement structure
const struct device *const sht = DEVICE_DT_GET_ANY(sensirion_sht4x); // Get SHT sensor

int8_t temp_value1, temp_value2; // Temp values
int8_t hum_value1, hum_value2;   // Humidity values
int err, counter = 0;            // Error code and counter

// Advertising parameters
#define ADV_MIN_INTERVAL 0x30 // Min interval (3s)
#define ADV_MAX_INTERVAL 0x30 // Max interval (3s)
struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(
    BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
    ADV_MIN_INTERVAL, ADV_MAX_INTERVAL, NULL);

// Start parameters
#define ADV_DURATION 2 // Advertising duration (2 events)
struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, ADV_DURATION);

static uint8_t mfg_data[MFG_DATA_SIZE] = {0}; // Manufacturer data buffer
static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, MFG_DATA_SIZE), // Advertisement data
};

// Set random static Bluetooth address
static void set_random_static_address(void)
{
    printk("Starting iBeacon Demo\n");
    bt_addr_le_t addr;
    err = bt_addr_le_from_str(BT_ADDRESS, "random", &addr); // Set random address
    if (err)
    {
        printk("Invalid BT address (err %d)\n", err);
        return;
    }
    err = bt_id_create(&addr, NULL); // Create identity with address
    if (err)
    {
        printk("Creating new ID failed (err %d)\n", err);
        return;
    }
    printk("Created new address\n");
}

// Initialize Bluetooth advertisement
void adv_param_init(void)
{
    err = bt_le_ext_adv_create(&adv_param, NULL, &adv); // Create advertisement
    if (err)
    {
        printk("Failed to create advertising set (err %d)\n", err);
        return;
    }
    printk("Created extended advertising set\n");
}

// Fetch temperature and humidity data
void fetch_temp_data(void)
{
    mfg_data[0] = 0x03; // Set manufacturer data
    mfg_data[1] = 0x00;
    mfg_data[2] = 0x101;

    for (int i = 0; i < NUM_SENSOR_READINGS; i++)
    { // Fetch sensor data loop
        struct sensor_value temp, hum;

        err = sensor_sample_fetch(sht); // Fetch sensor samples
        if (!err)
        { // Get temperature and humidity if successful
            sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp);
            sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum);
            printf("Temperature= %f; Humidity= %f\n", sensor_value_to_double(&temp), sensor_value_to_double(&hum));

            temp_value1 = temp.val1;         // Temp integer part
            temp_value2 = temp.val2 / 10000; // Temp fraction part
            hum_value1 = hum.val1;           // Humidity integer part
            hum_value2 = hum.val2 / 10000;   // Humidity fraction part

            // Store temp and humidity data into buffer
            mfg_data[i * 4 + 3] = temp_value1;
            mfg_data[i * 4 + 4] = temp_value2;
            mfg_data[i * 4 + 5] = hum_value1;
            mfg_data[i * 4 + 6] = hum_value2;
        }
        else
        {
            printf("ERROR: Temperature data update failed: %d\n", err);
        }

        k_msleep(TEMP_FETCH_INTERVAL_MS); // Sleep for defined interval
    }

    // Print buffer and increment counter
    for (int mfgIdx = 0; mfgIdx < MFG_DATA_SIZE; mfgIdx++)
    {
        printf("%d ", mfg_data[mfgIdx]);
    }

    counter++;   // Increment counter
    start_adv(); // Start BLE advertisement
}

// Start Bluetooth advertisement
void start_adv(void)
{
    err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0); // Set advertisement data
    if (err)
    {
        printk("Failed (err %d)\n", err);
        return;
    }

    printk("\nStart Extended Advertising...");
    err = bt_le_ext_adv_start(adv, &ext_adv_param); // Start extended advertisement
    if (err)
    {
        printk("Failed to start extended advertising (err %d)\n", err);
        return;
    }
    printk("done.\n");
    k_msleep(ADVERTISING_STOP_DELAY_MS); // Sleep for defined delay
    bt_le_ext_adv_stop(adv);             // Stop advertisement
    printk("Stopped advertising..!!\n");
    printf("No of times data published: %d\n", counter); // Log counter
    fetch_temp_data(); // Fetch sensor data after advertising
}

void main(void)
{
    printk("Starting Temperature and Humidity Node\n");

    set_random_static_address(); // Set random BLE address

    err = bt_enable(NULL); // Enable Bluetooth stack
    if (err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    adv_param_init();  // Initialize advertising parameters
    fetch_temp_data(); // Fetch sensor data
}
