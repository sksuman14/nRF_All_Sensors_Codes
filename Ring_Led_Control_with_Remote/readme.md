
# Ring LED with Remote Project

## Overview

This repository contains code for controlling a Ring LED connected to an nRF52832 board using a BLE-enabled remote. When the remote's button is pressed, the Ring LED on the board lights up via Bluetooth Low Energy (BLE) communication.

## Features

- Controls a Ring LED on a receiver board using a remote node over BLE.
- Simple BLE-based wireless control to toggle the Ring LED on and off with a button press.
- Configurable pin and power setup for the Ring LED.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832 or another Zephyr-compatible BLE board (for the receiver).
- Ring LED (connected to the receiver board).
- BLE Node (remote) to send control signals.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and set up. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for installation instructions.
2. **BLE Configuration**: Enable BLE support in Zephyr to allow communication between the remote node and receiver.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832 board and remote node.
2. **Enable BLE Support**: Ensure BLE configurations are enabled in `prj.conf` for both the remote node and receiver.
3. **Build the Project**: Build and flash the code for both the remote and receiver.
4. **Pairing Remote and Receiver**: Set up pairing between the remote and receiver to establish BLE communication.

## Ring LED Pin Configuration

| Ring LED Pin | NRF52832 Pin       |
|--------------|---------------------|
| VDD          | 5V (external power) |
| GND          | GND                 |
| DI           | P0.23               |

## How It Works

1. **Button Press on Remote**: When a button is pressed on the BLE node (remote), a signal is sent to the receiver board.
2. **Ring LED Control on Receiver**: The receiver listens for BLE signals and toggles the Ring LED based on the button state from the remote.

## Usage

After flashing the board, power on both the remote and receiver. When the button on the remote is pressed once , the Ring LED on the receiver board will light up. when the button will pressd again the Ring LED is turn off.

## Troubleshooting

### 1. BLE Connection Issues

- Confirm that both devices are in range and powered.
- Check that BLE pairing was successful.
- Verify the BLE settings match on both devices.

### 2. Ring LED Not Responding

- Confirm the Ring LED pin configuration on the receiver.
- Ensure the remote is sending signals correctly when the button is pressed.
