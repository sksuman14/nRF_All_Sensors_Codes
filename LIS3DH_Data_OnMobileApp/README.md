
# Accelerometer on Mobile App Project

## Overview

This repository contains code for streaming accelerometer data from an nRF52832DK node to a mobile app via Bluetooth Low Energy (BLE). This setup enables real-time monitoring of accelerometer readings on a mobile device.

## Features

- Continuously reads accelerometer data from the sensor.
- Transmits accelerometer data to a connected mobile app over BLE.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible BLE board.
- Accelerometer sensor (e.g., LIS3DH) connected to the board.
- Mobile device with a BLE-enabled app to receive and display data.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. Mobile app capable of receiving BLE data (e.g., nRF Connect for Mobile, custom mobile app)
4. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
5. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Make sure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup instructions.
2. **BLE Configuration**: Enable BLE support in Zephyr to allow data transmission to the mobile app.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable I2C and BLE Support**: In the `prj.conf` file, ensure I2C for the accelerometer and BLE are enabled.
3. **Build the Project**: Build and flash the code for the nRF52832DK.
4. **Pairing with Mobile App**: Pair the device with a BLE-capable mobile app that can receive and display the accelerometer data.

## How It Works

1. **Accelerometer Reading**: The nRF52832DK continuously reads data from the accelerometer sensor.
2. **Data Transmission over BLE**: The board transmits accelerometer data to a mobile app over BLE.
3. **Real-Time Display on Mobile App**: The mobile app displays the accelerometer readings in real-time.

## Output show on mobile APP

## Usage

Once the board is flashed, power on the nRF52832DK and open a BLE-compatible app on the mobile device. Pair the board with the app to start receiving live accelerometer data.

## Troubleshooting
### 1. BLE Connection Issues

- Confirm that both devices are within BLE range and powered.
- Check that BLE pairing was successful.
- Ensure the BLE settings are correctly configured in `prj.conf`.

### 2. Data Not Displaying on Mobile App

- Verify the accelerometer is connected properly and configured in the device tree.
- Confirm the mobile app is set up to receive accelerometer data.
- Enable GPS and Bluetooth in the mobile.

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.

![Screenshot 2024-12-06 165225](https://github.com/user-attachments/assets/b9888fcd-70de-4122-b4c9-33bf19fff890)

