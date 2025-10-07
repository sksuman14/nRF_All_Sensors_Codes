#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <stdio.h>

const char *displayText = "Hello World";

void main(void)
{
    const struct device *dev = device_get_binding("SSD1306");

    if (!dev) {
        printk("OLED Display: Device not found\n");
        return;
    }

    if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0 ||
        cfb_framebuffer_init(dev) != 0) {
        printk("OLED Display: Initialization failed\n");
        return;
    }

    cfb_framebuffer_clear(dev, true);
    cfb_framebuffer_invert(dev);
    display_blanking_off(dev);

    uint8_t fontWidth, fontHeight;
    cfb_get_font_size(dev, 0, &fontWidth, &fontHeight);

    uint16_t start_x = (cfb_get_display_parameter(dev, CFB_DISPLAY_WIDTH) - strlen(displayText) * fontWidth) / 2;
    uint16_t start_y = (cfb_get_display_parameter(dev, CFB_DISPLAY_HEIGH) - fontHeight) / 2;

    if (cfb_print(dev, displayText, start_x, start_y)) {
        printk("OLED Display: Failed to print string\n");
        return;
    }

    cfb_framebuffer_finalize(dev);
}
