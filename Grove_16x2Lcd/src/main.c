/*  Grove 16*2 LCD interfacing using NRF52832 Board*/

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/misc/grove_lcd/grove_lcd.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// Function to reverse a string
void reverse_string(char *str)
{

        int len = strlen(str);
        str[len] = '\0'; // Null-terminate the string
        for (int i = 0; i < len / 2; i++)
        {
                char temp = str[i];
                str[i] = str[len - 1 - i];
                str[len - 1 - i] = temp;
        }
}

int main(void)
{
        // Get the handle to the grove LCD device
        const struct device *const glcd = DEVICE_DT_GET(DT_NODELABEL(glcd));

        // Check if the device is ready
        if (!device_is_ready(glcd))
        {
                printk("Grove LCD: Device not ready.\n");
                return 0;
        }

        // Configure the LCD
        glcd_function_set(glcd, GLCD_FS_ROWS_2 | GLCD_FS_DOT_SIZE_LITTLE | GLCD_FS_8BIT_MODE);
        glcd_display_state_set(glcd, GLCD_DS_DISPLAY_ON | GLCD_DS_CURSOR_ON | GLCD_DS_BLINK_ON);

        // Original message
        char message[16] = " Hello world ";

        /* Reverse the message*/
        reverse_string(message);

        // Print reversed message on LCD
        glcd_cursor_pos_set(glcd, 0, 1); // Move cursor to second row
        glcd_print(glcd, message, strlen(message));

        return 0;
}
