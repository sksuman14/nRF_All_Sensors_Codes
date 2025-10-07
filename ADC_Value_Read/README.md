# ADC Reading Project

## Overview

This repository contains code to interface with the Analog-to-Digital Converter (ADC) on an nRF52832DK board using Zephyr RTOS. The project reads analog voltage values and displays the results via a serial interface.

## Features

- Reads analog voltage using the ADC peripheral.
- Displays ADC values over a serial interface.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- Analog sensor or voltage source.
- Connection wires.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **ADC Support**: Ensure ADC support is enabled in the Zephyr configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable ADC Support**: In the `prj.conf` file, enable ADC communication.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the Analog Sensor**: Connect the sensor or voltage source to an ADC pin on the nRF52832DK.

## ADC Pin Configuration

| ADC Pin   | NRF52832 Pin     |
|-----------|------------------|
| Analog In | P0.04      |
| GND       | GND              |

## How It Works

1. **Analog Signal Measurement**: The nRF52832DK reads the voltage level on the ADC input pin.
2. **Data Processing and Output**: The ADC values are converted and displayed over the serial interface.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
ADC Value: 1023 mV

```

## Usage

Once the firmware is flashed, the nRF52832DK will continuously read analog voltage levels using the ADC and display the readings over a serial interface.

## Troubleshooting

### 1. No ADC Data Displayed

- Verify the analog sensor or voltage source connection.
- Check the device tree configuration to ensure the ADC is correctly defined.
- Ensure ADC communication is enabled in the project configuration.

### 2. Incorrect ADC Readings

- Double-check the ADC pin mapping.
- Ensure the input voltage does not exceed the ADC reference voltage.
- Confirm proper grounding and stable power supply.

