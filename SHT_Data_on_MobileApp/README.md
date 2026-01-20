
# SHT40 on Mobile App Project

## Overview

This repository contains code for transmitting temperature and humidity data from an SHT40 sensor connected to an nRF52832DK node. Data is streamed to a mobile app in real time using Bluetooth Low Energy (BLE).

## Features

- Continuously reads temperature and humidity data from the SHT40 sensor.
- Sends sensor data to a mobile app via BLE.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible BLE board.
- SHT40 temperature and humidity sensor.
- Mobile device with a BLE-enabled app to receive and display data.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. Mobile app capable of receiving BLE data (e.g., nRF Connect for Mobile, custom mobile app)
4. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
5. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. See the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **BLE Configuration**: Enable BLE support in Zephyr to allow data streaming to the mobile app.

## Setup

1. **Configure the Build Environment**: Set up the environment for your nRF52832DK board.
2. **Enable I2C and BLE Support**: In the `prj.conf` file, enable I2C for the SHT40 sensor and BLE communication.
3. **Build the Project**: Build and flash the code for the nRF52832DK.
4. **Pair with Mobile App**: Pair the device with a BLE-compatible mobile app to receive temperature and humidity data.

## How It Works

1. **Data Acquisition from SHT40**: The nRF52832DK reads temperature and humidity data from the SHT40 sensor.
2. **BLE Data Transmission**: The board transmits the data to a mobile app over BLE.
3. **Live Data Display on Mobile App**: The mobile app displays the temperature and humidity readings in real time.

## Output show on Mobile APP

## Usage

After flashing, power on the nRF52832DK and open a BLE-compatible mobile app. Pair the board with the app to start receiving temperature and humidity data from the SHT40 sensor.

## Troubleshooting


### 1. BLE Connection Issues

- Ensure both devices are within BLE range and powered.
- Confirm BLE pairing was successful.
- Verify BLE configurations in `prj.conf`.

### 2. No Data on Mobile App
- Make sure GPS Must be enable in mobile, so that mobile is capable to search nearby BLE devices.
- Check the SHT40 sensor connection and device tree configuration.
- Ensure the mobile app is set up to receive temperature and humidity data.

## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.

![Screenshot 2024-12-06 165125](https://github.com/user-attachments/assets/002fa0c4-6cd2-4654-9717-65e0321ff457)
