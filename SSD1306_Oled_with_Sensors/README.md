# OLED Display with LIS3DH, VCNL4040 and SHT40 Sensors


## Overview:

This project demonstrates how to interface an SSD1306 OLED display with both the VCNL4040 proximity and ambient light sensor and the SHT40 temperature and humidity sensor. The OLED display will show readings from these sensors in real-time.

## Features:
- Display X,Y,Z value from LIS3DH sensor.
- Display temperature and humidity from the SHT40 sensor.
- Display proximity and ambient light readings from the VCNL4040 sensor.
- Use of an SSD1306 OLED display for visual output.
- Example code for each sensor combination.

##Hardware Requirements:

- Microcontroller (e.g., NRF52832, NRF52840 etc.)
- SSD1306 OLED display (128x64 or 128x32)
- LIS3DH sensor
- VCNL4040 sensor
- SHT40 sensor
- Jumper wires
- Breadboard (optional)

## Software Requirements:

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Pin Configuration:
  
| Sensor's/Oled pin | Microcontroller Pin |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |

## Usage

- Connect the SSD1306 OLED display, LIS3DH,VCNL4040 and SHT40 sensors to your microcontroller according to the pin configuration.
- Run the example code.
- You should see the sensor readings displayed on the OLED screen.




















