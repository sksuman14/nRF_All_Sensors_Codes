#include <stdio.h>
#include <string.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/zephyr.h>

// Define flash memory configuration
#define spiFlashTestRegionOffset 0xff000 // Flash memory offset
#define spiFlashSectorSize 4096          // Flash sector size
#define flashName "W25R64JV"             // Flash device name
#define delayTime 1000                   // Delay time in ms

// Macro for error checking
#define CHECK_ERROR(rc, msg)                \
    if (rc != 0)                            \
    {                                       \
        printf("%s failed! %d\n", msg, rc); \
        return;                             \
    }

void main(void)
{
    const uint8_t expected[] = {0x55}; // Expected data
    // const uint8_t expected[] = {0x55, 0xaa, 0x66, 0x99}; // Expected data
    const size_t len = sizeof(expected);                 // Length of data
    uint8_t buf[len];                                    // Buffer for reading data

    const struct device *flashDev = DEVICE_DT_GET(DT_ALIAS(spi_flash0)); // Get flash device
    if (!device_is_ready(flashDev))                                      // Check if device is ready
    {
        printf("%s: device not ready.\n", flashDev->name); // Print error if not ready
        return;
    }

    printf("\n%s SPI flash testing\n", flashDev->name); // Print flash test message
    printf("==========================\n");

    printf("\nTest 1: Flash erase\n");
    int rc = flash_erase(flashDev, spiFlashTestRegionOffset, spiFlashSectorSize); // Erase flash sector
    CHECK_ERROR(rc, "Flash erase");                                               // Check erase error

    printf("\nTest 2: Flash write\n");
    printf("Attempting to write %zu bytes\n", len);                      // Print write attempt message
    rc = flash_write(flashDev, spiFlashTestRegionOffset, expected, len); // Write to flash
    CHECK_ERROR(rc, "Flash write");                                      // Check write error

    memset(buf, 0, len);                                           // Clear buffer
    rc = flash_read(flashDev, spiFlashTestRegionOffset, buf, len); // Read from flash
    CHECK_ERROR(rc, "Flash read");                                 // Check read error

    if (memcmp(expected, buf, len) == 0) // Compare data
    {
        printf("Data read matches data written. Good job!\n"); // Print success
        for (size_t i = 0; i < len; i++)                       // Print data
        {
            printf("%02x ", buf[i]); // Print byte
        }
        printf("\n");
    }
    else
    {
        printf("Data read does not match data written!\n"); // Print mismatch
        for (size_t i = 0; i < len; i++)                    // Print mismatched data
        {
            printf("%08x wrote %02x read %02x %s\n", // Print mismatch details
                   (uint32_t)(spiFlashTestRegionOffset + i), expected[i], buf[i],
                   (expected[i] == buf[i]) ? "match" : "MISMATCH");
        }
    }

    k_msleep(delayTime); // Delay before next operation
}
