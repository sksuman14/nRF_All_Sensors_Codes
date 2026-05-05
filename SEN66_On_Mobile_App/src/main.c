
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/controller.h>

#define I2C_NODE DT_NODELABEL(sen66)

static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

#define SEN66_ADDR 0x6B // I2C address of the SEN66 sensor

#define CMD_START_MEAS 0x0021
#define CMD_READ_VALUES 0x0300
#define CMD_RESET 0xD304

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

struct bt_le_ext_adv *adv;

struct bt_le_adv_param adv_param = BT_LE_ADV_PARAM_INIT(
    BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_EXT_ADV |
        BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_NO_2M,
    0x30, 0x30, NULL);

struct bt_le_ext_adv_start_param ext_adv_param = BT_LE_EXT_ADV_START_PARAM_INIT(0, 0);

static uint8_t mfg_data[21] = {0};

static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 21),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

int err;
/* Function to send a command to the SEN66 sensor over I2C */
void send_command(uint16_t cmd)
{
        uint8_t buf[2];
        buf[0] = (cmd >> 8); // Extract High Byte
        buf[1] = cmd & 0xFF; // Extract Low Byte

        i2c_write_dt(&dev_i2c, buf, sizeof(buf));
}

/* Function to read measurement data from the SEN66 sensor over I2C */
void read_data(uint16_t *output, uint8_t data_size)
{
        // Each measurement data point consists of 2 bytes + 1 CRC byte (3 bytes total).
        // For 9 data points, we read 27 bytes.
        uint8_t data_array[27];
        i2c_read_dt(&dev_i2c, data_array, sizeof(data_array));

        uint8_t i = 0;
        while (i < data_size)
        {
                // Assemble MSB and LSB while skipping the 3rd byte (CRC) of every triplet
                uint8_t msb = data_array[i * 3];
                uint8_t lsb = data_array[i * 3 + 1];
                output[i] = (msb << 8) | lsb;
                i++;
        }
}
/* Function to initialize Bluetooth advertising parameters and create an advertising set */
void adv_param_init(void)
{
        err = bt_le_ext_adv_create(&adv_param, NULL, &adv);
        if (err)
        {
                printk("Failed to create advertising, err %d\n", err);
        }
        else
        {
                printk("Created extended advertising\n");
        }
}

/* Function to set a random static Bluetooth address for the device */
void set_random_static_address(void)
{
        bt_addr_le_t addr;

        err = bt_addr_le_from_str("DE:AD:BE:AF:BA:01", "random", &addr);
        if (err != 0)
        {
                printk("Invalid BT address, err %d\n", err);
        }

        err = bt_id_create(&addr, NULL);
        if (err < 0)
        {
                printk("Creating new id failed, err %d\n", err);
        }
        else
        {
                printk("Created new address with id %d\n", err);
                adv_param.id = err;
        }
}
/* Function to start Bluetooth advertising with the configured parameters and data */
void start_adv(void)
{
        // printk("Start advertising\n");

        err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
        if (err)
        {
                printk("Failed to set extended advertising data, err %d\n", err);
        }

        printk("Start Extended Advertising...\n");
        err = bt_le_ext_adv_start(adv, &ext_adv_param);
        if (err)
        {
                printk("Failed to start advertising, err %d\n", err);
        }

        k_msleep(500);
        bt_le_ext_adv_stop(adv);
        printk("Stopped advertising\n");
}
/* Function to perform fan cleaning by sending specific commands to the SEN66 sensor and waiting for the required durations */
void fan_cleaning()
{
        // 1. Stop measurement mode (Cmd: 0x0104)
        send_command(0x0104);
        k_msleep(10); // Wait for sensor to process stop command

        // 2. Start fan cleaning (Cmd: 0x5607)
        send_command(0x5607);
        k_msleep(10);

        // 3. Restart measurement mode
        send_command(CMD_START_MEAS);
        k_msleep(10); // Warm-up delay

        /* ---countdown --- */
        for (int i = 0; i <= 100; i++)
        {
                printk("\rWaiting for Fan cleaning: %3d%%", i);
                k_msleep(80);
        }
        printk("\n");

        printk("Fan cleaning done, resumes taking reading \n");
}

/* Main function to initialize the device, check I2C bus readiness, and continuously read sensor data, update Bluetooth advertising data, and perform periodic fan cleaning */
int main(void)
{
        if (!device_is_ready(dev_i2c.bus))
        {
                printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
                return -1;
        }
        else
        {
                printk("Sensor is working\n");
        }

        printk("SEN66 on Mobile App\n");
        
        uint8_t bt_err = bt_enable(NULL);
        if (bt_err)
        {
                printk("Bluetooth init failed, error %d\n", bt_err);
                return -1;
        }
        set_random_static_address();
        /* Initialize Bluetooth advertising parameters and create an advertising set */
        adv_param_init();

        send_command(CMD_RESET);
        k_msleep(10);

        fan_cleaning();
        uint32_t last_read_time = 0;

        send_command(CMD_START_MEAS);
        k_msleep(10);

        uint16_t output[9];

        int64_t last_time = 0;
        /* Main loop to continuously read sensor data, update Bluetooth advertising data, and perform periodic fan cleaning every 24 hours */
        while (1)
        {
                // 24-Hour Maintenance: Trigger fan cleaning daily
                static int64_t last_time = 0;
                if ((k_uptime_get_32() - last_time >= (24 * 60 * 60 * 1000)))
                {
                        fan_cleaning();
                        last_time = k_uptime_get_32();
                }

                // --- HARDWARE GUARD ---
                // SEN66 should not be polled faster than once per second to prevent I2C bus congestion or sensor errors.
                uint32_t now = k_uptime_get_32();
                if (now - last_read_time < 1000)
                {
                        k_msleep(1000 - (now - last_read_time));
                }
                /* Send command to read sensor values and wait for the required duration before reading the data */
                send_command(CMD_READ_VALUES);
                k_msleep(20);
                /* Clear the output buffer and read the raw sensor data from the SEN66 sensor over I2C */
                memset(mfg_data, 0, sizeof(mfg_data));
                read_data(output, 9);
                /* Process the raw sensor data and convert it to human-readable values */
                float pm1 = output[0] / 10.0f;
                float pm25 = output[1] / 10.0f;
                float pm4 = output[2] / 10.0f;
                float pm10 = output[3] / 10.0f;
                float rh = (int16_t)output[4] / 100.0f;
                float temp = (int16_t)output[5] / 200.0f;
                int16_t voc = (int16_t)output[6] / 10;
                int16_t nox = (int16_t)output[7] / 10;
                uint16_t CO2 = output[8];
                /* Update the Bluetooth advertising data with the latest sensor readings */
                mfg_data[0] = 0x59;
                mfg_data[1] = 0x00;

                /* Device ID */
                mfg_data[2] = 158;

                /* PM1.0 */
                mfg_data[3] = (output[0] >> 8) & 0xFF;
                mfg_data[4] = output[0] & 0xFF;

                /* PM2.5*/
                mfg_data[5] = (output[1] >> 8) & 0xFF;
                mfg_data[6] = output[1] & 0xFF;

                /* PM4.0 */
                mfg_data[7] = (output[2] >> 8) & 0xFF;
                mfg_data[8] = output[2] & 0xFF;

                /* PM10 */
                mfg_data[9] = (output[3] >> 8) & 0xFF;
                mfg_data[10] = output[3] & 0xFF;

                /* Temperature */
                mfg_data[11] = (output[5] >> 8) & 0xFF;
                mfg_data[12] = output[5] & 0xFF;

                /* Relative Humidity */
                mfg_data[13] = (output[4] >> 8) & 0xFF;
                mfg_data[14] = output[4] & 0xFF;

                // CO2 ppm
                mfg_data[15] = (CO2 >> 8) & 0xFF;
                mfg_data[16] = CO2 & 0xFF;

                // VOC index
                mfg_data[17] = (voc >> 8) & 0xFF;
                mfg_data[18] = voc & 0xFF;

                // NOx index
                mfg_data[19] = (nox >> 8) & 0xFF;
                mfg_data[20] = nox & 0xFF;

                /* Serial debug output */
                printk("PM1.0 : %.1f µg/m³\n", pm1);
                printk("PM2.5 : %.1f µg/m³\n", pm25);
                printk("PM4.0 : %.1f µg/m³\n", pm4);
                printk("PM10  : %.1f µg/m³\n", pm10);
                printk("RH    : %.2f %%\n", rh);
                printk("Temp  : %.2f °C\n", temp);
                printk("VOC   : %d\n", voc);
                printk("NOx   : %d\n", nox);
                printk("CO2   : %u ppm\n", CO2);
                printk("\n");
                /* Start Bluetooth advertising with the updated sensor data */
                start_adv();
                k_msleep(50);
        }

        return 0;
}
