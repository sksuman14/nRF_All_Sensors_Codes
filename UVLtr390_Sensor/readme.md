
---

# UV Sensor (LTR390) Project

## Overview

This repository contains code to interface an LTR390 UV sensor with an nRF52832DK board. The project measures ultraviolet (UV) light levels and provides real-time output.

## Features

- Interfaces with the LTR390 UV sensor over I2C.
- Continuously measures UV light intensity.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- LTR390 UV sensor.
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
2. **Enable I2C Support**: In the `prj.conf` file, ensure I2C communication is enabled.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the UV Sensor**: Connect the LTR390 UV sensor to the I2C pins on the nRF52832DK.

## LTR390 Pin Configuration

| LTR390 Pin | NRF52832 Pin     |
|------------|------------------|
| VCC        | 3.3V             |
| GND        | GND              |
| SDA        | P0.26 (I2C SDA)  |
| SCL        | P0.27 (I2C SCL)  |

## How It Works

1. **UV Light Measurement**: The nRF52832DK continuously reads UV light intensity data from the LTR390 sensor via I2C.
2. **Data Processing and Output**: The UV readings are displayed over the serial interface.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
UV Index: 1
UV Index: 2
UV Index: 3
```

## Usage

Once the firmware is flashed, the nRF52832DK will start measuring UV light levels using the LTR390 sensor and display the readings over a serial interface.

## Troubleshooting

### 1. No Data Displayed

- Verify the I2C connections between the board and the sensor.
- Check the device tree configuration to ensure the LTR390 is correctly defined.
- Ensure I2C is enabled in the project configuration.

### 2. Incorrect UV Measurements

- Double-check the I2C address of the LTR390 sensor.
- Ensure the sensor is correctly powered and connected.
- Ensure there are no physical obstructions affecting the sensor’s exposure to UV light.
