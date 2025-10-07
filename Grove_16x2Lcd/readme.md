

# 16x2 LCD Display Interface Project

## Overview

This repository contains code for interfacing a 16x2 LCD display with the nRF52832DK board using Zephyr RTOS. This example demonstrates printing a "Hello World" message on the display.

## Features

- Interfaces with a 16x2 LCD display over I2C.
- Displays a "Hello World" message on the screen.
- Configurable I2C pins for flexibility on Zephyr-supported boards.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- 16x2 LCD Display (I2C interface).
- Microcontroller/Board: NRF52832DK (or any other Zephyr-compatible platform with I2C support).

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and set up correctly. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for installation instructions.
2. **CMake**: Used for building the Zephyr project.
3. **LCD Display Driver**: Ensure that the I2C interface and LCD driver are included in the Zephyr device tree and enabled in the configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for the nRF52832DK board or any Zephyr-compatible board.
2. **Enable I2C and Display Support**: Ensure `I2C` and display configurations are enabled in the `prj.conf` file.
3. **Building the Project**: Build the project using `west`.
4. **Flashing the Firmware**: After a successful build, flash the firmware onto your board.

## Pin Configuration

| LCD Pin | NRF52832DK Pin |
|---------|----------------|
| VCC     | VCC            |
| GND     | GND            |
| SDA     | P0.27          |
| SCL     | P0.26          |

## Sample Output

```console
Hello World
```

## Usage

Once the board is flashed, the application will initialize the 16x2 LCD display and print "Hello World" on it. The display will update automatically after each reset.

## Troubleshooting

### 1. Display Not Working

- Ensure the display is connected properly and powered.
- Verify that the I2C pins (SDA and SCL) are correctly configured in your setup.
- Confirm that the I2C address matches the display module (common addresses are 0x27 or 0x3F).

### 2. Build or Flashing Issues

- Double-check the configuration in `prj.conf` and device tree bindings.
- Verify that your development environment is set up correctly according to the Zephyr guidelines.

