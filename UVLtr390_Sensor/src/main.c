/* For UV index Value*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define SLEEP_TIME_MS 1000

/* Define LTR390 I2C address and register addresses */
#define LTR390_I2C_ADDR 0x53
#define LTR390_MAIN_CTRL 0x00
#define LTR390_MEAS_RATE 0x04
#define LTR390_GAIN 0x05
#define LTR390_UVS_DATA_0 0x10
#define LTR390_UVS_DATA_1 0x11
#define LTR390_UVS_DATA_2 0x12
#define LTR390_MAIN_STATUS 0x07

/* Define measurement settings */
#define UVS_MODE 0x0A  // Set UVS mode (UVS active)
#define RESOLUTION_18BIT_TIME100MS 0x20
#define GAIN_3X 0x01

/* Get the node identifier of the sensor */
#define I2C0_NODE DT_NODELABEL(mysensor)

int main(void)
{
    int ret;

    /* Retrieve the API-specific device structure */
    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
    if (!device_is_ready(dev_i2c.bus))
    {
        printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
        return -1;
    }

    /* Set UVS mode in MAIN_CTRL register */
    uint8_t config_main[2] = {LTR390_MAIN_CTRL, UVS_MODE};
    ret = i2c_write_dt(&dev_i2c, config_main, sizeof(config_main));
    if (ret != 0)
    {
        printk("Failed to write to MAIN_CTRL register\n");
        return -1;
    }

    /* Configure measurement rate and resolution */
    uint8_t config_meas[2] = {LTR390_MEAS_RATE, RESOLUTION_18BIT_TIME100MS};
    ret = i2c_write_dt(&dev_i2c, config_meas, sizeof(config_meas));
    if (ret != 0)
    {
        printk("Failed to write to MEAS_RATE register\n");
        return -1;
    }

    /* Configure gain */
    uint8_t config_gain[2] = {LTR390_GAIN, GAIN_3X};
    ret = i2c_write_dt(&dev_i2c, config_gain, sizeof(config_gain));
    if (ret != 0)
    {
        printk("Failed to write to GAIN register\n");
        return -1;
    }

    while (1)
    {
        /* Check status if data is ready */
        uint8_t status = 0;
        ret = i2c_reg_read_byte_dt(&dev_i2c, LTR390_MAIN_STATUS, &status);
        if (ret != 0 || !(status & 0x08))
        {
            printk("Data not ready, waiting...\n");
            k_msleep(SLEEP_TIME_MS);
            continue;
        }

        /* Read UVS data (3 bytes) */
        uint8_t uv_reading[3] = {0};
        ret = i2c_burst_read_dt(&dev_i2c, LTR390_UVS_DATA_0, uv_reading, 3);
        if (ret != 0)
        {
            printk("Failed to read UVS data\n");
            return -1;
        }

        /* Combine the three bytes into a single UV index value */
        int uv_index = (uv_reading[2] << 16) | (uv_reading[1] << 8) | uv_reading[0];

        /* Print the UV index value */
        printk("UV Index: %d\n", uv_index);

        k_msleep(SLEEP_TIME_MS);
    }
}
