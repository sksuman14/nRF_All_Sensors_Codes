# I2C Scanner Project

This repository contains a simple I2C scanner application using Zephyr RTOS. The I2C scanner checks for connected devices on the I2C bus and prints their addresses to the console.


## Features:
 

- Scans the I2C bus for connected devices.
- Detects and displays I2C device addresses.
- Supports Zephyr RTOS.

## Hardware Requirements:
 

- Microcontroller/Board: Compatible with Zephyr RTOS and I2C (e.g., nRF52832).
- Devices connected to the I2C bus (e.g., sensors, displays).

## Software Requirements:
 

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites:
 

- Zephyr RTOS: Install Zephyr RTOS and set up your environment. Refer to the Zephyr Getting Started Guide for instructions.
- CMake: Needed for building the project.

## Setup
 

## 1. Configure the Build Environment:

Set up the environment for your specific board. For example, to build for an NRF52832 board.

## 2. Enable I2C Support:
 
Ensure the following configuration options are enabled in the prj.conf file.

## 3. Building the Project:
 
Build the project with the following command:

## 4. Flashing the Firmware:
 
Once the project is successfully built, you can flash the firmware onto your device.

## Usage:
 
After flashing the firmware, the program will start scanning the I2C bus for connected devices. The addresses of the detected devices will be printed to the serial console.


## Sensor Pin Connections


| Sensor Pin | Microcontroller Pin |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |



Sample Output:
=============

Here is a sample output from the I2C scanner:

.. code-block:: console

             *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138  ***
             Starting I2C Scanner...
             Device found at address: 0x39
             Device found at address: 0x39
             Device found at address: 0x39
             Scan complete.

## Troubleshooting:
 

## 1. No Devices Detected:
 
- Check the physical connections of the I2C devices (SDA, SCL, pull-up resistors, power, and ground).
- Ensure the I2C bus is enabled and configured correctly in the device tree.
- Use an oscilloscope or logic analyzer to check I2C bus activity.

## Build Errors:
 
- Ensure you have enabled I2C support in the prj.conf file.
- Verify your Zephyr environment is correctly set up.


