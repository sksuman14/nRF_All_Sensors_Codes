#  VCNL4040 Proximity and Ambient Light Sensor


## Overview

The VCNL4040 is an integrated proximity and ambient light sensor designed for a wide range of applications, including smartphones, tablets, and IoT devices. It features high sensitivity and a small form factor, making it ideal for detecting light levels and proximity in compact designs.

## Features
 
- Integrated proximity and ambient light sensing
- High accuracy and sensitivity
- Digital I2C interface
- Adjustable LED drive current
- Low power consumption

## Hardware Requirements
 
- Microcontroller (e.g., NRF52832, NRF52840, etc.)
- VCNL4040 sensor module
- Jumper wires
- Breadboard (optional)

## Software Requirements:
 
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## VCNL Sensor Pin Connections
 
| VCNL Pin | Microcontroller Pin  |
|----------|-----------------------|
| VCC      | 3.3V                 |
| GND      | GND                  |
| SDA      | I2C Data Line (SDA)  |
| SCL      | I2C Clock Line (SCL) |
               
# Sample Output

     Ambient Light: 67 lux  
     Ambient Light: 69 lux
     Ambient Light: 69 lux  

       

## Usage

- Connect the VCNL4040 sensor to your microcontroller according to the pin configuration above.
- Run the example code provided in the examples folder.
- The sensor readings will be displayed via the Serial Monitor.


## Troubleshooting
1. Sensor Not Detected:
   - Verify the I2C wiring and the sensor address (default: 0x60).
   - Ensure I2C is enabled in prj.conf.

2. Build Errors:
   - Double-check that the Zephyr environment is set up correctly.
   - Ensure the VCNL4040 driver and device tree bindings are properly included.
     
 ## Note :
 This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.












