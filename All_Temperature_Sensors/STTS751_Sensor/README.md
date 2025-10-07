# STTS751 Sensor Project

This repository contains code for interfacing the STTS751 digital temperature sensor with Zephyr RTOS on an embedded platform.


## Features
 
- Reads temperature data from the STTS751 sensor.
- Communicates via the I2C interface.
- Configurable temperature update rates and thresholds.
- Compatible with Zephyr RTOS.


## Hardware Requirements:

- STTS751 Sensor (I2C digital temperature sensor).
- Microcontroller/Board: Compatible with Zephyr RTOS and I2C (e.g., NRF52832).
- Optional: Display for showing temperature data.

## Software Requirements:
 
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites
 
- Zephyr RTOS: Install Zephyr RTOS and set up your environment. Refer to the Zephyr Getting Started Guide for instructions.
- CMake: Needed for building the project.
- STTS751 Sensor Driver: Ensure that the sensor driver is included and enabled in the project configuration.


## Setup:
 
- Configure the Build Environment: Set up the environment for your specific board. For example, to build for an NRF52832 board.
- Enable STTS751 Support: In the prj.conf file, ensure the necessary configuration options are set.
- Building the Project: Build the project with the following command.
- Flashing the Firmware: After a successful build, flash the firmware onto the device.


## Usage:
 
Once flashed, the program will start reading temperature data from the STTS751 sensor at regular intervals. The temperature readings will be displayed via the serial console or an optional display.


## STS751 Sensor Pin Connections

| STS751 Pin | Microcontroller Pin  |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |


## Sample Output:
 
.. code-block:: console

          *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138  ***
          Temperature: 24.75°C
          Temperature in Fahrenheit: 75.63 F
          Temperature: 24.75°C 
          Temperature in Fahrenheit: 75.63 F
          Temperature: 24.75°C  
          Temperature in Fahrenheit: 75.63 F

## Optional: Display on OLED or LCD
 
You can configure the system to display the temperature on an OLED or LCD screen. Ensure the appropriate driver (e.g., SSD1306 for OLED) is enabled in the Zephyr configuration.

## Troubleshooting
1.Sensor Not Detected:

- Ensure the sensor is connected correctly and the I2C address (0x39) is properly set in the device tree.
- Check that the I2C bus is configured and enabled in your project.

2.Build Issues:

- Ensure the STTS751 sensor driver is correctly enabled in the prj.conf.
- Verify that your Zephyr environment is set up correctly.

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.
