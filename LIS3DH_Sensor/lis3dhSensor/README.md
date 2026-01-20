# LIS3DH Accelerometer Sensor with Zephyr RTOS
 
This project demonstrates how to use the LIS3DH accelerometer sensor with Zephyr RTOS. It reads acceleration data and logs it over a serial interface.

## Hardware Requirements:
 
1. Nordic nRF52-based development board (e.g., nRF52832 or nRF52840)
2. LIS3DH accelerometer sensor module
3. Necessary wiring and pull-up resistors (if required)

## Features:
 
1. Read acceleration data from the LIS3DH sensor.
2. Log acceleration values to the console.
3. Configure sensor settings (e.g., sensitivity, output data rate).


## Software Requirements:
 

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

 
## LIS3DH Accelerometer Pin Connections

| LIS3DH Pin | Microcontroller Pin  |
|------------|----------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | I2C Data Line       |
| SCL        | I2C Clock Line      |



## Configuration:
 
Ensure that you have the appropriate configuration settings in prj.conf for your project.


## Setup and Usage:
 
1. Clone the repository to your local machine.
2. Modify the prj.conf and overlay.dts files as necessary for your hardware setup.
3. Build the application using the Zephyr build system:
4. Flash the application onto your board:
5. Monitor the output via a serial console:

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.
















