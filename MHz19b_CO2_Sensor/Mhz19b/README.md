# CO2 Sensor Project
 
Description
***********

This sample application demonstrate the configurations of the MH-Z19B CO2
sensor, then periodically reads CO2 data from the sensor. The driver
currently only support uart interrupt APIs.

## Features:

- Reads CO2 data from the MH-Z19B sensor.
- Uses the UART interface for communication.
- Compatible with Zephyr RTOS.


Requirements
************

The baudrate of the UART must be configured to 9600, and must support
interrupt driven API.

## Software Requirements:
 
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)


## Prerequisites:
 

1. Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. You can refer to the Zephyr Getting Started Guide for installation and setup instructions.
2. CMake: Used for building the Zephyr project.
3. CO2 Sensor Driver: The driver should be included in the device tree and enabled in the configuration.

## Setup:
 

1. Configure the Build Environment: Before building the project, set up the environment for your board. For example, if you're using an NRF52832 board.
2. Enable CO2 Support: In the prj.conf file, ensure you have enabled I2C and sensor support.
3. Building the Project: To build the project.
4. Flashing the Firmware: After a successful build, flash the firmware onto your board.

## Pin Configuration:
 
## CO2 Sensor Pin Connections

| CO2 Pin   | Microcontroller Pin  |
|-----------|----------------------|
| VCC       | 5V                  |
| GND       | GND                 |
| RX        | TX                  |
| TX        | RX                  |


Sample Output
=============

The application will perform runtime configuration of the sensor, then
read back the sensor configuration data. After that it will read the CO2
data every 1 seconds.

.. code-block:: console

    sensor: co2 reading: 758
    sensor: co2 reading: 759
    sensor: co2 reading: 762


   <repeats endlessly every 1 seconds>
