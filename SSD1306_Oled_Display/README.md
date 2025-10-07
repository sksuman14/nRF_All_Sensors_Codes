# SSD1306 OLED Display Project

## Overview
This project demonstrates how to interface an SSD1306 OLED display with a microcontroller using I2C or SPI. The SSD1306 is a monochrome display that can be used to display text, images, and graphics.

## Features
- Display text and graphics on the OLED screen.
- Support for various fonts.
- Example usage of I2C and SPI communication.

## Hardware Requirements
- Microcontroller (e.g., NRF52832, NRF52840, etc.)
- SSD1306 OLED display (128x64 or 128x32)
- Jumper wires
- Breadboard (optional)

## Software Requirements:
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Pin Configuration:
| OLED Pin | Microcontroller Pin  |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |


## Usage:
- Connect the SSD1306 OLED display to your microcontroller according to the pin configuration above.
- Run the sample code.
- You should see text and graphics displayed on the OLED screen.

  
## Troubleshooting:
 
1. No Devices Detected:
- Check the physical connections of the OLED Display (SDA, SCL, power, and ground).
- Ensure the I2C bus is enabled and configured correctly in the device tree.
- Use an oscilloscope or logic analyzer to check I2C bus activity.

2. Build Errors:
- Ensure you have enabled I2C and OLED support in the prj.conf file.
- Verify your Zephyr environment is correctly set up.

3.OLED Display Issues:
- Ensure the display driver (e.g., SSD1306) is enabled and configured correctly.
- Verify the resolution matches your display (e.g., 128x64 or 128x32).

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.






























