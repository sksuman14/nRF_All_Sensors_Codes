# Stepper Motor Control Project (28BYJ-48)

## Overview

This repository contains code to interface and control a 28BYJ-48 stepper motor using an nRF52832DK board with Zephyr RTOS. The project uses GPIO and PWM signals to drive the motor through a ULN2003AN driver board.

## Features

- Controls the 28BYJ-48 stepper motor using GPIO or PWM.
- Supports direction and step control.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- 28BYJ-48 stepper motor.
- ULN2003AN driver board.
- Connection wires.
- External power supply (if required by the motor).

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **GPIO/PWM Support**: Ensure GPIO and PWM support are enabled in the Zephyr configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable GPIO/PWM Support**: In the `prj.conf` file, enable the required peripherals.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the Stepper Motor**: Connect the 28BYJ-48 stepper motor to the ULN2003AN driver board and wire it to the nRF52832DK.

## Stepper Motor Pin Configuration

| ULN2003AN Pin | NRF52832 Pin |
|-------------|--------------|
| IN1         | P0.24        |
| IN2         | P0.25        |
| IN3         | P0.26        |
| IN4         | P0.27        |
| VCC         | 5V           |
| GND         | GND          |

## How It Works

1. **Step Sequence Generation**: The nRF52832DK sends step sequences to the ULN2003AN driver board to rotate the motor.
2. **Speed and Direction Control**: The step speed and direction can be adjusted programmatically.
3. **Step Execution**: The motor moves according to the step sequence sent from the microcontroller.


## Usage

Once the firmware is flashed, the nRF52832DK will control the 28BYJ-48 stepper motor by sending appropriate signals to the ULN2003AN driver board.

## Troubleshooting

### 1. Stepper Motor Not Moving

- Verify the wiring between the ULN2003AN driver and the nRF52832DK.
- Ensure the correct step sequence is used.
- Check if GPIO and PWM support are enabled in the project configuration.

### 2. Incorrect Motor Rotation

- Ensure the step sequence follows the correct order.
- Verify the motor’s power supply voltage and ensure sufficient current supply.
- Check if any connections are loose or incorrect.

