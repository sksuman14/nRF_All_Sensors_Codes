#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

/* --- Configuration & Definitions --- */
#define I2C_NODE DT_NODELABEL(sen66)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

#define SEN66_ADDR 0x6B
#define CMD_START_MEAS 0x0021
#define CMD_READ_VALUES 0x0300
#define CMD_RESET 0xD304

/* --- Function Definitions --- */

void send_command(uint16_t cmd)
{
    uint8_t buf[2];
    buf[0] = (cmd >> 8);
    buf[1] = cmd & 0xFF;
    
     i2c_write_dt(&dev_i2c, buf, sizeof(buf));
}

void read_data(uint16_t *output, uint8_t data_size)
{
    uint8_t data_array[27]; 

    i2c_read_dt(&dev_i2c, data_array, sizeof(data_array));

    for (uint8_t i = 0; i < data_size; i++)
    {
        uint8_t msb = data_array[i * 3];
        uint8_t lsb = data_array[i * 3 + 1];
        output[i] = (msb << 8) | lsb;
    }
}

void fan_cleaning()
{
    send_command(0x0104); 
    k_msleep(1400);

    send_command(0x5607); 
    k_sleep(K_SECONDS(10));

    send_command(CMD_START_MEAS); 
    k_msleep(2000);

    printk("Fan cleaning done, resumed measurements\n");
}

int main(void)
{
    if (!device_is_ready(dev_i2c.bus))
    {
        printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
        return -1;
    }

    printk("Sensor is working..\n");
    printk("Waiting for Fan cleaning...\n");

    /* --- Initialization --- */
    send_command(CMD_RESET);
    k_msleep(1200);

    send_command(CMD_START_MEAS);
    k_msleep(2000);

    fan_cleaning();

    uint16_t output[9];
    uint32_t last_read_time = 0;

    /* --- Burn initial stale data --- */
    send_command(CMD_READ_VALUES);
    k_msleep(20);
    read_data(output, 9);
    last_read_time = k_uptime_get_32();

    int64_t last_clean_time = k_uptime_get_32();

    while (1)
    {
        /* --- Daily fan cleaning --- */
        if ((k_uptime_get_32() - last_clean_time) >= (24 * 60 * 60 * 1000))
        {
            fan_cleaning();
            last_clean_time = k_uptime_get_32();
        }

        /* --- Ensure ≥1 second polling interval --- */
        uint32_t now = k_uptime_get_32();
        if (now - last_read_time < 1000)
        {
            k_msleep(1000 - (now - last_read_time));
        }

        /* --- Read sensor data --- */
        send_command(CMD_READ_VALUES);
        k_msleep(20);
        read_data(output, 9);
        last_read_time = k_uptime_get_32();

        /* --- Convert Data --- */
        float pm1  = output[0] / 10.0f;
        float pm25 = output[1] / 10.0f;
        float pm4  = output[2] / 10.0f;
        float pm10 = output[3] / 10.0f;
        float rh   = (int16_t)output[4] / 100.0f;
        float temp = (int16_t)output[5] / 200.0f;
        int16_t voc = (int16_t)output[6] / 10;
        int16_t nox = (int16_t)output[7] / 10;
        uint16_t CO2 = output[8];

        /* --- Print Data --- */
        printk("Uptime: %d s\n", k_uptime_get_32() / 1000);
        printk("PM1.0: %.2f | PM2.5: %.2f µg/m³\n", pm1, pm25);
        printk("RH: %.2f %% | Temp: %.2f °C\n", rh, temp);
        printk("VOC: %d | NOx: %d\n", voc, nox);
        printk("CO2: %d ppm\n\n", CO2);

        /* --- Fixed delay (1 second) --- */
        k_msleep(1000);
    }

    return 0;
}