# Servo Motor Control Project

## Overview

This repository contains code to interface a servo motor with an nRF52832DK board using Zephyr RTOS. The project controls the servo motor position using PWM signals and allows for angle adjustments.

## Features

- Controls a servo motor using PWM.
- Allows precise angle adjustments.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- Servo motor (e.g., SG90 or MG995).
- External power supply (if required by the servo motor).
- Connection wires.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **PWM Support**: Ensure PWM support is enabled in the Zephyr configuration.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable PWM Support**: In the `prj.conf` file, enable PWM communication.
3. **Build the Project**: Build and flash the code onto the nRF52832DK board.
4. **Connect the Servo Motor**: Connect the servo motor to a PWM-capable pin on the nRF52832DK.

## Servo Motor Pin Configuration

| Servo Pin  | NRF52832 Pin     |
|------------|------------------|
| VCC        | 3.3V or 5V       |
| GND        | GND              |
| Signal     | P0.27 (PWM OUT)  |

## How It Works

1. **PWM Signal Generation**: The nRF52832DK generates a PWM signal to control the servo motor position.
2. **Angle Adjustment**: The duty cycle of the PWM signal determines the servo angle.
3. **Movement Execution**: The servo motor moves to the corresponding angle based on the PWM pulse width.

## Usage

Once the firmware is flashed, the nRF52832DK will control the servo motor using PWM signals. The servo angle can be adjusted programmatically.

## Troubleshooting

### 1. Servo Motor Not Moving

- Verify the PWM pin configuration.
- Ensure the servo motor is receiving sufficient power.
- Check if PWM support is enabled in the project configuration.

### 2. Incorrect Servo Positioning

- Ensure the PWM duty cycle is correctly set.
- Verify the servo motor specifications for correct PWM pulse width values.
- Make sure there are no physical obstructions preventing servo movement.

