# SHT40, VCNL4040, and OLED Display Project
This repository contains code for integrating the SHT40 temperature and humidity sensor, VCNL4040 proximity and ambient light sensor, and an OLED screen to display sensor readings using Zephyr RTOS on an embedded platform.

## Features:
- SHT40 Sensor: Reads temperature and humidity data.
- VCNL4040 Sensor: Reads proximity and ambient light data.
- OLED Display: Displays sensor data from both sensors.
- Uses the I2C interface for communication.
- Configurable update rates for sensor readings.
- Compatible with Zephyr RTOS.

## Hardware Requirements:
- Sensors:
  - SHT40 (I2C temperature and humidity sensor).
  - VCNL4040 (I2C proximity and ambient light sensor).
- Display:
  - OLED Screen (e.g., SSD1306 or compatible).
- Microcontroller:
 - NRF52832 (or any other Zephyr-compatible platform with I2C support).

## Software Requirements:
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites:
1. Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. Refer to the Zephyr Getting Started Guide for installation instructions.
2. SHT40 and VCNL4040 Sensor Drivers: Include and enable the drivers in the device tree and configuration files.
3. OLED Display Driver: Enable the SSD1306 driver (or the driver for your specific OLED) in the configuration.

## Setup:
1. Configure the Build Environment: Set up the environment for your board, e.g., NRF52832.
2. Enable Sensor and OLED Support: Update the prj.conf file to enable I2C, sensor, and OLED display support.
3. Building the Project: Build the project using Zephyr's build tools.
4. Flashing the Firmware: Flash the firmware onto your board after a successful build.

## Pin Configuration:

| SHT40/VCNL4040/OLED Pin | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 3.3V                |
| GND       | GND                 |
| SDA       | I2C Data Line       |
| SCL       | I2C Clock Line      |



## Sample Output:
Serial Console Output:

     *** Booting Zephyr OS build v2.6.0-rc1-315-g50d8d1187138 ***  
     SHT40: Temp: 23.64°C, Humidity: 30.74%  
     VCNL4040: Ambient Light: 456.78 lx  
     SHT40: Temp: 23.66°C, Humidity: 32.16%  
     VCNL4040: Ambient Light: 459.23 lx  


## OLED Display Example:
The OLED screen will display:

      Temp: 23.64°C  
      Humidity: 30.74%  
      Ambient: 456.78 lx  



## Usage:
1. Flash the firmware onto your board.
   
2. Upon startup, the application will:
  - Read temperature and humidity data from the SHT40 sensor.
  - Read proximity and ambient light data from the VCNL4040 sensor.
  - Display the data on the OLED screen.
  - Log the data to the serial console.
  
      
## Troubleshooting:
1.Sensor or OLED Not Detected:

- Ensure the connections are correct.
- Verify the I2C addresses for SHT40 (default: 0x44), VCNL4040 (default: 0x60), and OLED display.
- Ensure I2C is enabled in the configuration.

2.Build Issues:

- Double-check the configuration and device tree bindings.
- Verify the environment setup as per the Zephyr project guidelines.

3.OLED Display Issues:

- Ensure the display driver (e.g., SSD1306) is enabled and configured correctly.
- Verify the resolution matches your display (e.g., 128x64 or 128x32).
      
