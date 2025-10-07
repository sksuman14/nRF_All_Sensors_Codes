
# Magnetometer (TLV493D) Project

## Overview

This repository contains code to interface a TLV493D magnetometer with an nRF52832DK board using I2C communication. The project reads magnetic field data from the TLV493D and provides real-time output.

## Features

- Interfaces with the TLV493D 3D magnetometer over I2C.
- Continuously reads magnetic field data.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- TLV493D magnetometer.
- I2C connection cables.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **I2C Support**: Make sure I2C is enabled in the Zephyr configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable I2C Support**: In the `prj.conf` file, ensure I2C communication is enabled.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the Magnetometer**: Connect the TLV493D magnetometer to the I2C pins on the nRF52832DK.

## TLV493D Pin Configuration

| TLV493D Pin | NRF52832 Pin     |
|-------------|------------------|
| VCC         | 3.3V             |
| GND         | GND              |
| SDA         | P0.26 (I2C SDA)  |
| SCL         | P0.27 (I2C SCL)  |

## How It Works

1. **Magnetic Field Reading**: The nRF52832DK continuously reads magnetic field data from the TLV493D sensor using I2C.
2. **Data Processing and Output**: The magnetic field values are processed and can be printed to a serial console or used for further applications.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
Magnetic Field (µT) - X: 0.23, Y: -0.15, Z: 0.50
```

## Usage

Once the firmware is flashed, the nRF52832DK starts reading data from the TLV493D magnetometer. The data is displayed over the serial interface.

## Troubleshooting

### 1. No Data Displayed

- Verify the I2C connections between the board and the magnetometer.
- Check the device tree configuration and ensure the TLV493D is correctly defined.
- Ensure I2C is enabled in the configuration.

### 2. Incorrect Data Readings

- Double-check the I2C address of the TLV493D sensor.
- Ensure the sensor is correctly powered and connected.
