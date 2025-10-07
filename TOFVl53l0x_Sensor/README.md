
# Time of Flight (TOF) VL53L0X Sensor Project

## Overview

This repository contains code to interface a VL53L0X Time of Flight (TOF) sensor with an nRF52832DK board. The project measures distances using the VL53L0X sensor and displays the data via a serial interface.

## Features

- Interfaces with the VL53L0X TOF sensor over I2C.
- Measures and displays distance data.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- VL53L0X TOF distance sensor.
- I2C connection cables.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **I2C Support**: Ensure I2C communication is enabled in the Zephyr configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable I2C Support**: In the `prj.conf` file, enable I2C communication.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the TOF Sensor**: Connect the VL53L0X TOF sensor to the I2C pins on the nRF52832DK.

## VL53L0X Pin Configuration

| VL53L0X Pin | NRF52832 Pin     |
|-------------|------------------|
| VCC         | 3.3V             |
| GND         | GND              |
| SDA         | P0.26 (I2C SDA)  |
| SCL         | P0.27 (I2C SCL)  |

## How It Works

1. **Distance Measurement**: The nRF52832DK continuously reads distance data from the VL53L0X TOF sensor via I2C.
2. **Data Processing and Output**: The distance measurements are displayed over the serial interface for monitoring.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
Distance is 20 cm
```

## Usage

Once the firmware is flashed, the nRF52832DK will start measuring distances using the VL53L0X TOF sensor and display the readings over a serial interface.

## Troubleshooting

### 1. No Distance Data Displayed

- Verify the I2C connections between the board and the sensor.
- Check the device tree configuration to ensure the VL53L0X is correctly defined.
- Ensure I2C communication is enabled in the project configuration.

### 2. Incorrect Distance Measurements

- Double-check the I2C address of the VL53L0X sensor.
- Ensure the sensor is correctly powered and connected.
- Make sure there are no obstructions near the sensor affecting its measurements.

