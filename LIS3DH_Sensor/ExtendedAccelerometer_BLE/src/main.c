#include <zephyr/bluetooth/bluetooth.h> // Include Bluetooth header
#include <zephyr/zephyr.h>              // Include Zephyr kernel API
#include <zephyr/drivers/gpio.h>        // Include GPIO driver API
#include <zephyr/device.h>              // Include device driver API
#include <zephyr/pm/pm.h>               // Include power management API
#include <hal/nrf_gpio.h>               // Include Nordic GPIO HAL
#include <zephyr/drivers/sensor.h>      // Include sensor driver API

/* Define global constants */
#define SENSOR_TYPE 0x1E                   // Sensor type
#define ADV_DURATION_MS 20000              // Advertisement duration (ms)
#define FETCH_INTERVAL_MS 500              // Fetch interval (ms)
#define MOTION_DATA_SIZE 80                // Motion data buffer size
#define MFG_DATA_SIZE 246                  // Manufacturer data buffer size
#define SENSOR_ADDRESS "FA:17:DF:DF:4B:89" // Static Bluetooth Address

/* Define pin numbers */
#define LED0_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios)   // LED0 pin
#define LED2_PIN DT_GPIO_PIN(DT_NODELABEL(led2), gpios)   // LED2 pin
#define BUTTON0_PIN DT_GPIO_PIN(DT_NODELABEL(button0), gpios) // Button0 pin

typedef struct
{
  uint8_t xLsb; // X-axis LSB
  uint8_t yLsb; // Y-axis LSB
  uint8_t zLsb; // Z-axis LSB
} MotionData_t;

/* Declare global variables */
int counter = 1;                                             // Initialize counter
static struct bt_le_ext_adv *adv;                          // Advertising structure
static const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh); // Sensor device
static MotionData_t motionData[MOTION_DATA_SIZE];          // Motion data buffer
static uint8_t mfgData[MFG_DATA_SIZE];                     // Manufacturer data buffer

static uint32_t lastTime = 0;      // Last button press time
static uint32_t buttonCounter = 0; // Button press counter
static bool sleepFlag = true;      // Sleep flag
static int motionIndex = 0;        // Current motion data index
static int accFetchFlag = 0;       // Accelerometer fetch flag
static int err = 0;                // Error flag

struct bt_le_ext_adv_start_param extAdvParam = BT_LE_EXT_ADV_START_PARAM_INIT(0, 2); // Advertisement parameters

/* Helper functions */
static inline void enterSleep(void)
{
  printk("Entering SYSTEM-OFF mode\n");
  NRF_POWER->SYSTEMOFF = 1; // Enter power-off mode
}

static inline double roundOff(float value)
{
  return (value > 0) ? (value + 0.5f) : (value - 0.5f); // Round off float
}

static void setRandomStaticAddress(void)
{
  bt_addr_le_t addr;
  err = bt_addr_le_from_str(SENSOR_ADDRESS, "random", &addr); // Convert address string
  if (err)
  {
    printk("Invalid BT address (err %d)\n", err); // Address conversion error
    return;
  }

  err = bt_id_create(&addr, NULL); // Create Bluetooth ID
  if (err < 0)
  {
    printk("Creating new ID failed (err %d)\n", err); // ID creation error
  }
  else
  {
    printk("Created new address\n"); // Success
  }
}

static void advParamInit(void)
{
  struct bt_le_adv_param advParam = BT_LE_ADV_PARAM_INIT(
      BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
      BT_GAP_ADV_FAST_INT_MIN_2, BT_GAP_ADV_FAST_INT_MAX_2, NULL); // Advertisement parameters

  err = bt_le_ext_adv_create(&advParam, NULL, &adv); // Create advertisement set
  if (err)
  {
    printk("Failed to create advertising set (err %d)\n", err); // Creation error
  }
  else
  {
    printk("Created extended advertising set\n"); // Success
  }
}

static void fetchAccData(void)
{
  struct sensor_value accel[3]; // Accelerometer data

  if (!device_is_ready(sensor))
  {
    printk("Device %s is not ready\n", sensor->name); // Sensor not ready
    return;
  }

  for (int motionIdx = 0; motionIdx < MOTION_DATA_SIZE; motionIdx++)
  {
    if (sensor_sample_fetch(sensor) == 0)
    {
      if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel) == 0)
      {
        motionData[motionIdx].xLsb = (int8_t)(roundOff(accel[0].val1 * 6.4 + accel[0].val2 * 0.0000064));
        motionData[motionIdx].yLsb = (int8_t)(roundOff(accel[1].val1 * 6.4 + accel[1].val2 * 0.0000064));
        motionData[motionIdx].zLsb = (int8_t)(roundOff(accel[2].val1 * 6.4 + accel[2].val2 * 0.0000064));
      }
    }
    printf("X=%d Y=%d Z=%d\n", motionData[motionIdx].xLsb, motionData[motionIdx].yLsb, motionData[motionIdx].zLsb);
    k_msleep(100);
    accFetchFlag++;
    if (accFetchFlag >= MOTION_DATA_SIZE)
    {
      accFetchFlag = 0; // Reset flag
    }
  }
}

void repeatedAdv(void)
{
  uint8_t *accPtr = (uint8_t *)&motionData[motionIndex]; // Pointer to motion data
  mfgData[0] = SENSOR_TYPE; // Set sensor type

  for (int mfgIdx = 1; mfgIdx <= 240; mfgIdx++)
  {
    mfgData[mfgIdx] = *(accPtr++); // Copy motion data
  }

  motionIndex += MOTION_DATA_SIZE; // Update index
  if (motionIndex >= MOTION_DATA_SIZE)
  {
    motionIndex = 0; // Reset index
  }

  struct bt_data ad[] = {
      BT_DATA(BT_DATA_MANUFACTURER_DATA, mfgData, sizeof(mfgData)), // Manufacturer data
  };

  err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0); // Set advertising data
  if (err)
  {
    printk("Failed (err %d)\n", err); // Set data error
  }
}

void startAdv(void)
{
  struct bt_data ad[] = {
      BT_DATA(BT_DATA_MANUFACTURER_DATA, mfgData, sizeof(mfgData)), // Manufacturer data
  };

  err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0); // Set advertising data
  if (err)
  {
    printk("Failed to set advertising data (err %d)\n", err); // Set data error
    return;
  }

  err = bt_le_ext_adv_start(adv, &extAdvParam); // Start advertisement
  if (err)
  {
    printk("Failed to start extended advertising (err %d)\n", err); // Start error
  }
  else
  {
    printk("Advertising started.\n"); // Success
    bt_le_ext_adv_stop(adv);   // Stop advertisement
    printk("Stopped advertising.\n"); // Stop success
    printk("Entering sleep mode...\n"); // Enter sleep mode
    printk("Number of times data published: %d\n", counter); // Print counter
    k_msleep(ADV_DURATION_MS); // Sleep during advertisement
    counter++; // Increment counter
  }
}

void main(void)
{
  nrf_gpio_cfg_output(LED2_PIN); // Configure LED2 as output
  nrf_gpio_pin_set(LED2_PIN);    // Turn on LED2

  nrf_gpio_cfg_input(BUTTON0_PIN, NRF_GPIO_PIN_PULLUP);        // Configure Button0 as input with pull-up
  nrf_gpio_cfg_sense_set(BUTTON0_PIN, NRF_GPIO_PIN_SENSE_LOW); // Set Button0 to sense low

  printk("Button status=%d\n", nrf_gpio_pin_read(BUTTON0_PIN)); // Print button status

  while (nrf_gpio_pin_read(BUTTON0_PIN) == 0)
  {
    uint32_t time = k_uptime_get(); // Get current uptime
    if (buttonCounter == 0)
    {
      lastTime = time; // Record time of first press
    }

    if (time - lastTime >= 5000) // Check 5 seconds press
    {
      buttonCounter = 0; // Reset counter
      sleepFlag = false; // Clear sleep flag
      printk("Button pressed for 5 seconds..\n"); // Print message

      for (int ledIdx = 0; ledIdx <= 5; ledIdx++)
      {
        nrf_gpio_cfg_output(LED0_PIN); // Configure LED0 as output
        nrf_gpio_pin_toggle(LED0_PIN); // Toggle LED0
        k_sleep(K_MSEC(50)); // Sleep for 50ms
      }
      break; // Exit loop
    }
    buttonCounter++; // Increment counter
  }

  if (sleepFlag)
  {
    enterSleep(); // Enter sleep mode
  }

  nrf_gpio_pin_set(LED0_PIN); // Turn on LED0
  printk("Starting Periodic Advertising Demo\n"); // Print start message
  setRandomStaticAddress(); // Set random static address

  err = bt_enable(NULL); // Initialize Bluetooth
  if (err)
  {
    printk("Bluetooth init failed (err %d)\n", err); // Initialization error
    return;
  }

  advParamInit(); // Initialize advertising parameters
  k_msleep(3000); // Sleep for initialization

  while (1)
  {
    fetchAccData(); // Fetch accelerometer data
    repeatedAdv(); // Prepare advertisement data
    startAdv();    // Start advertisement
  }
}
