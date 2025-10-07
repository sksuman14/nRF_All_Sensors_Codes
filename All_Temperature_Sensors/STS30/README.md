# STS30 Sensor Project
 

## This repository contains code for integrating and using the STS30 temperature sensor with Zephyr RTOS on an embedded platform.
 

## Features:
 

- Reads temperature data from the STS30 sensor.
- Uses the I2C interface for communication.
- Configurable update rates for sensor readings.
- Displays sensor data on a connected display (e.g., OLED).
- Compatible with Zephyr RTOS.

## Hardware Requirements:
 
- STS30 Sensor (I2C temperature sensor).
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
3. STS30 Sensor Driver: The driver should be included in the device tree and enabled in the configuration.

## Setup:
 

1. Configure the Build Environment: Before building the project, set up the environment for your board. For example, if you're using an NRF52832 board.
2. Enable STS30 Support: In the prj.conf file, ensure you have enabled I2C and sensor support.
3. Building the Project: To build the project.
4. Flashing the Firmware: After a successful build, flash the firmware onto your board.


## Pin Configuration:
 
| STS30 Pin | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 3.3V                |
| GND       | GND                 |
| SDA       | I2C Data Line       |
| SCL       | I2C Clock Line      |

Sample Output:
==============
.. code-block:: console

        *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138  ***
        STS30: 23.64 Temp. [C] 
        STS30: 23.66 Temp. [C] 
        STS30: 23.63 Temp. [C] 


## Usage:
 

Once the board is flashed, the application will start reading temperature data from the STS30 sensor at regular intervals and print the results over the serial console.

## Optional: Display Data on OLED:
 

You can also display the data on an OLED connected to the board. This functionality is optional and requires the SSD1306 OLED driver to be enabled in Zephyr.

## Troubleshooting:
 

## 1. Sensor Not Detected:
 
- Ensure the sensor is connected properly and the I2C address (0x4a/0x4b by default) is correct.
- Verify that I2C is enabled in your configuration.

## 2. Build Issues:
 
- Double-check the configuration and device tree bindings.
- Ensure your environment is set up correctly as per the Zephyr project guidelines.
