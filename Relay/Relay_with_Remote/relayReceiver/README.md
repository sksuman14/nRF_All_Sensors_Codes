# BLE Node/Receiver for Relay Control

## Overview:
This project provides the code for a BLE transmitter/Node that sends a unique ID to a BLE receiver. Upon receiving this ID, the receiver toggles a relay. This setup enables wireless control of a relay using BLE communication.

## Features:
- BLE Receiver: Receive a unique identifier periodically.
- Relay Control: Toggles the relay on the receiver board when the unique ID is detected.
- Secure Communication: Ensures only the designated ID toggles the relay.
- Compatible with Zephyr RTOS for seamless integration.

## Hardware Requirements:
- Transmitter Board: nRF52832 or any Zephyr-compatible BLE board.
- Receiver Board: nRF52832 or similar with a connected relay module.
- Relay Module: Compatible with the receiver board's GPIO pins.

## Software Requirements
1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. Development Environment: IDE supporting Zephyr (e.g., VS Code with Zephyr Extension)
4. CMake and west (for building Zephyr projects)

## Prerequisites
1. Zephyr RTOS Setup: Install and configure Zephyr RTOS. Follow the Zephyr Getting Started Guide.
2. BLE Configuration: Ensure BLE is enabled in prj.conf for both the transmitter and receiver.

## Setup
Receiver
1. Connect the relay module to the receiver board. Use the following GPIO configuration.
2. Build and flash the receiver firmware.
3. Pair the receiver with the transmitter to establish BLE communication.

## Relay Pin Configuration

| Relay Pin | NRF52832 Pin       |
|--------------|---------------------|
| VDD          | 5V                  |
| GND          | GND                 |
| IN1          | P0.18               |

## How It Works
1. Transmitter:
- The BLE transmitter broadcasts a unique ID periodically or when triggered.
- The ID is sent as part of the manufacturer data in the advertising packet.

2.Receiver:
- The receiver listens for BLE advertisements.
- When the unique ID is detected in the received packet, the receiver toggles the connected relay.

## Troubleshooting
1. BLE Connection Issues
   - Ensure both devices are powered and in range.
   - Check that the BLE configurations match in prj.conf.
2. Relay Not Toggling
   - Verify the relay's pin configuration.
   - Ensure the relay module is receiving adequate power.
   - Check the logs on the receiver for errors.
