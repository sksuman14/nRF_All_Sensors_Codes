# SHT40 Sensor and OLED Display Project
This repository contains code for integrating and using the SHT40 temperature and humidity sensor along with an OLED screen to display sensor readings using Zephyr RTOS on an embedded platform.

## Features:
- Reads temperature and humidity data from the SHT40 sensor.
- Uses the I2C interface for communication.
- Displays sensor data on a connected OLED screen (e.g., SSD1306).
- Configurable update rates for sensor readings.
- Compatible with Zephyr RTOS.

## Hardware Requirements:
- SHT40 Sensor (I2C temperature and humidity sensor).
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
- SHT40 Sensor Driver: Ensure the driver is included in the device tree and enabled in the configuration.
- OLED Display Driver: Enable the SSD1306 driver (or the driver for your specific OLED) in the configuration.

## Setup:
- Configure the Build Environment: Set up the environment for your board, e.g., NRF52832.
- Enable SHT40 and OLED Support: Update the prj.conf file to enable I2C, sensor, and OLED display support.
- Building the Project: Build the project using Zephyr's build tools.
- Flashing the Firmware: Flash the firmware onto your board after a successful build.


## Pin Configuration:

| SHT40/OLED Pin | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 3.3V                |
| GND       | GND                 |
| SDA       | I2C Data Line       |
| SCL       | I2C Clock Line      |


Sample Output:

          *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138 ***  
          SHT40: Temp: 23.64°C, Humidity: 30.74%  
          SHT40: Temp: 23.66°C, Humidity: 32.16%  
          SHT40: Temp: 23.63°C, Humidity: 30.83%  


## OLED Display Example:
The OLED screen will display:
        
        Temp: 23.64°C  
        Humidity: 30.74%  



## Usage:
Once the board is flashed, the application will start reading temperature and humidity data from the SHT40 sensor at regular intervals and display the results on the OLED screen. The data will also be logged to the serial console.

## Troubleshooting:
Sensor or OLED Not Detected:

1.Ensure the connections are correct.
- Verify the I2C addresses for the SHT40 (0x44 by default) and OLED display.
- Ensure I2C is enabled in the configuration.
  
2.Build Issues:

- Double-check the configuration and device tree bindings.
- Verify the environment setup as per the Zephyr project guidelines.

3.OLED Display Issues:

- Ensure the display driver (e.g., SSD1306) is enabled and configured correctly.
- Verify the resolution matches your display (e.g., 128x64 or 128x32).











