# SHT40 Sensor Project
 

## This repository contains code for integrating and using the SHT40 temperature and humidity sensor with Zephyr RTOS on an embedded platform.


## Features:
 

- Reads temperature and humidity data from the SHT40 sensor.
- Uses the I2C interface for communication.
- Configurable update rates and thresholds for sensor readings.
- Displays sensor data on a connected display (e.g., OLED).
- Compatible with Zephyr RTOS.

## Hardware Requirements:
 
- SHT40 Sensor (I2C temperature and humidity sensor).
- Microcontroller/Board: NRF52832 (or any other Zephyr-compatible platform with I2C support).
- Optional: OLED display to visualize data.

## Software Requirements:
 

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites:
 

1. Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. You can refer to the Zephyr Getting Started Guide for installation and setup instructions.
2. CMake: Used for building the Zephyr project.
3. SHT40 Sensor Driver: The driver should be included in the device tree and enabled in the configuration.

## Setup:
 

1. Configure the Build Environment: Before building the project, set up the environment for your board. For example, if you're using an NRF52832 board.
2. Enable SHT40 Support: In the prj.conf file, ensure you have enabled I2C and sensor support.
3. Building the Project: To build the project.
4. Flashing the Firmware: After a successful build, flash the firmware onto your board.


## Pin Configuration:
 
## SHT40 Sensor Pin Connections

| SHT40 Pin | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 3.3V                |
| GND       | GND                 |
| SDA       | I2C Data Line       |
| SCL       | I2C Clock Line      |



Sample Output:
==============
.. code-block:: console

        *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138  ***
        SHT4X: 23.64 Temp. [C] ; 30.74 RH [%] -- 
        SHT4X: 23.66 Temp. [C] ; 32.16 RH [%] --
        SHT4X: 23.63 Temp. [C] ; 30.83 RH [%] -- 


## Usage:
 

Once the board is flashed, the application will start reading temperature and humidity data from the SHT40 sensor at regular intervals and print the results over the serial console.

## Optional: Display Data on OLED:
 

You can also display the data on an OLED connected to the board. This functionality is optional and requires the SSD1306 OLED driver to be enabled in Zephyr.

## Troubleshooting:


1. Sensor Not Detected:
- Ensure the sensor is connected properly and the I2C address (0x44 by default) is correct.
- Verify that I2C is enabled in your configuration.

2. Build Issues:
- Double-check the configuration and device tree bindings.
- Ensure your environment is set up correctly as per the Zephyr project guidelines.

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.
