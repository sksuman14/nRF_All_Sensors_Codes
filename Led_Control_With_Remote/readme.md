
# LED with Remote Project

## Overview

This repository contains code for controlling an LED on a board using a remote node. When a button is pressed on the remote, the LED on the board lights up via Bluetooth Low Energy (BLE) communication.

## Features

- Controls an LED on a receiver board using a remote node over BLE.
- Uses BLE for low-power wireless communication.
- Simple, responsive design to toggle LED on/off with button press.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- BLE-Enabled Board (e.g., NRF52832DK or any Zephyr-compatible BLE board).
- BLE Node (remote) to send control signals.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and set up correctly. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for installation instructions.
2. **BLE Configuration**: Enable BLE support in Zephyr to allow communication between the remote node and receiver.

## Setup

1. **Configure the Build Environment**: Set up the environment for your BLE-enabled board and remote node.
2. **Enable BLE Support**: In the `prj.conf` file, ensure BLE configurations are enabled for both the remote node and receiver board.
3. **Building the Project**: Build and flash the code for both the remote and receiver.
4. **Pairing Remote and Receiver**: Set up pairing between the remote and receiver, ensuring BLE communication is established.

## How It Works

1. **Button Press on Remote**: When a button is pressed on the node (remote), a signal is sent to the receiver.
2. **LED Control on Receiver**: The receiver listens for BLE signals and toggles the LED based on the button state from the remote.

## Pin Configuration

- LED Pin on Receiver: Configurable via code.
- Button Pin on Remote: Configurable via code.

## Usage

Once the board is flashed, power on both the remote and receiver. When the remote's button is pressed, the LED on the receiver board will turn on. Releasing the button will turn off the LED.

## Troubleshooting

### 1. BLE Connection Issues

- Ensure both devices are in range and properly powered.
- Confirm that BLE pairing was successful.
- Check the configuration to ensure BLE settings match on both devices.

### 2. LED Not Responding

- Verify the LED pin configuration on the receiver board.
- Check if the remote is sending signals correctly when the button is pressed.



