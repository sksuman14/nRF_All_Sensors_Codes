# BME680 Sensor Project
This repository contains code for integrating the BME680 environmental sensor with Zephyr RTOS on an embedded platform.


## Features:
- Reads data from the BME680 sensor, including:
  - Temperature
  - Humidity
  - Pressure
- Gas resistance (for air quality analysis).
- Uses the I2C or SPI interface for communication.
- Configurable update rates and thresholds for sensor readings.
- Compatible with Zephyr RTOS.

## Hardware Requirements:
- BME680 Sensor (environmental sensor with I2C/SPI support).
- Microcontroller/Board: NRF52832 (or any other Zephyr-compatible platform with I2C/SPI support).

## Software Requirements:
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites:
1. Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. Refer to the Zephyr Getting Started Guide for installation instructions.
2. BME680 Sensor Driver:  Include and enable the BME680 driver in the device tree and configuration files.
3.I2C/SPI Interface: Enable and configure the I2C or SPI interface based on your setup.

## Setup:
1. Configure the Build Environment: Set up the environment for your board, e.g., NRF52832.
2. Enable BME680 Support: Update the prj.conf file to enable I2C/SPI and sensor support.
3. Building the Project: Build the project using Zephyr's build tools.
4. Flashing the Firmware: Flash the firmware onto your board after a successful build.


## Pin Configuration:
|**I2C Mode**                      |
| BME680 Pin | Microcontroller Pin |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |



| **SPI Mode**                     |
| BME680 Pin | Microcontroller Pin |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| CS         | SPI Chip Select     |
| SCK        | SPI Clock           |
| SDI/SDO    | SPI MOSI/MISO Line  |



## Sample Output

    *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138 ***  
    Device 0x20002b74 name is BME680
    T: 23.988877; P: 97.648568; H: 53.689533; G: 1035.211466
    T: 24.168500; P: 97.648866; H: 53.565966; G: 1046.677896
    T: 24.336533; P: 97.648596; H: 53.353663; G: 1058.656533


## Usage:
1. Flash the firmware onto your board.
2. Upon startup, the application will:
   - Read temperature, humidity, pressure, and gas resistance data from the BME680 sensor.
   - Log the sensor readings to the serial console at regular intervals.

## Troubleshooting:
1. Sensor Not Detected:
   - Verify the connections and ensure the correct I2C address (default: 0x76 or 0x77) or SPI settings are used.
   - Ensure the I2C or SPI interface is enabled in the configuration.

2. Build Issues:
   - Check the device tree bindings and configuration settings.
   - Ensure the BME680 driver is enabled in the prj.conf file.
 
3. Incorrect Readings:
   - Ensure the sensor is calibrated and placed in a suitable environment for accurate readings.

   
