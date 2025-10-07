#include <lvgl.h>  // Include LittlevGL library for GUI
#include <zephyr/device.h>  // Device handling in Zephyr
#include <zephyr/kernel.h>  // Kernel and sleep functions
#include <zephyr/drivers/display.h>  // Display driver API
#include <zephyr/logging/log.h>  // Logging functionality

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL   
LOG_MODULE_REGISTER(app);  // Initialize logging module

void main(void) {                             
    const struct device *display_dev;  // Pointer for display device
    lv_obj_t *hello_world_label;  // Label object for display
    char* msg = "Hello, Zephyr\n";  // Message to display

    // Get the display device from the Device Tree
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    // Check if the display device is ready
    if (!device_is_ready(display_dev)) {                            
        LOG_ERR("Display device not ready");
        return;  // Exit if the display is not ready
    }

    // Turn on the display (unblank if it was blanked)
    display_blanking_off(display_dev);  
    
    // Create label object on the active screen
    hello_world_label = lv_label_create(lv_scr_act());  
    
    // Set the label text to the message
    lv_label_set_text(hello_world_label, msg);  
    
    // Align the label to the center of the display
    lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);  

    // Log the display update action
    printk("Display updated with message: %s\n", msg);  

    // Main loop for updating the display
    while (1) {     
        lv_task_handler();  // Handle LVGL tasks (required for rendering)
        // k_msleep(10);  // Sleep for 10ms to allow proper scheduling
    }
}
