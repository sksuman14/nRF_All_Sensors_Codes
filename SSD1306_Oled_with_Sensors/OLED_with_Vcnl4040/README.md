# OLED Screen and VCNL4040 Sensor Project

This repository contains code for integrating and using an OLED screen to display data from the VCNL4040 proximity and ambient light sensor with Zephyr RTOS on an embedded platform.

## Features:

- Reads proximity and ambient light data from the VCNL4040 sensor.
- Uses the I2C interface for communication.
- Displays sensor data on a connected OLED screen.
- Configurable update rates and thresholds for sensor readings.
- Compatible with Zephyr RTOS.

## Hardware Requirements:

- VCNL4040 Sensor (I2C proximity and ambient light sensor).
- OLED Screen (e.g., SSD1306 or compatible).
- Microcontroller/Board: NRF52832 (or any other Zephyr-compatible platform with I2C support).

## Software Requirements:

- Zephyr RTOS
- Nordic SDK (if using Nordic boards)
- An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
- CMake, west (for building Zephyr projects)


## Prerequisites:

- Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. Refer to the Zephyr Getting Started Guide for installation instructions.
- CMake: Used for building the Zephyr project.
- VCNL4040 Sensor Driver: Ensure the driver is included in the device tree and enabled in the configuration.
- OLED Display Driver: Enable the SSD1306 driver (or the driver for your specific OLED) in the configuration.

## Setup:

- Configure the Build Environment: Set up the environment for your board, e.g., NRF52832.
- Enable VCNL4040 and OLED Support: Update the prj.conf file to enable I2C, sensor, and OLED display support.
- Building the Project: Build the project using Zephyr's build tools.
- Flashing the Firmware: Flash the firmware onto your board after a successful build.


##  Pin Configuration:

| VCNL4040 Pin/OLED Pin | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 3.3V                |
| GND       | GND                 |
| SDA       | I2C Data Line       |
| SCL       | I2C Clock Line      |



Sample Output:
==============

       *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138 ***  
       VCNL4040:  Ambient Light: 456.78 lx  
       VCNL4040:  Ambient Light: 459.23 lx  
       VCNL4040:  Ambient Light: 457.89 lx  



## OLED Display Example:
The OLED screen will display:
      
     Ambient: 456.78 lx  


## Usage:
Once the board is flashed, the application will start reading proximity and ambient light data from the VCNL4040 sensor and display the results on the OLED screen. Data will also be logged to the serial console.


##  Troubleshooting:


1. Sensor or OLED Not Detected:
 - Ensure the connections are correct.
 - Verify the I2C addresses for the VCNL4040 (0x60 by default) and OLED display.
 - Ensure I2C is enabled in the configuration.

2. Build Issues:
- Double-check the configuration and device tree bindings.
- Verify the environment setup as per the Zephyr project guidelines.

3. OLED Display Issues:
- Ensure the display driver (e.g., SSD1306) is enabled and configured correctly.
- Verify the resolution matches your display (e.g., 128x64 or 128x32).


