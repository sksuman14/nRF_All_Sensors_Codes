# Ultrasonic Sensor Node

## Overview

This repository contains code for an **Ultrasonic Sensor Node**, which utilizes an **nRF52832** board and an **ultrasonic sensor** (e.g., HC-SR04) to measure distance. This setup is ideal for applications such as obstacle detection, proximity sensing, or liquid level monitoring.

## Features

- **Distance Measurement:** Uses an ultrasonic sensor to measure distances to objects.

## Hardware Requirements

- **nRF52832DK** or any Zephyr-compatible BLE board.
- **Ultrasonic Sensor** (e.g., HC-SR04, connected via GPIO pins).

## Software Requirements

1. **Zephyr RTOS**
2. **Nordic SDK** (if using Nordic boards)
3. **An IDE supporting Zephyr development** (e.g., VS Code with Zephyr Extension)
4. **CMake, west** (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS Setup:** Install and configure Zephyr by following the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).
2. **GPIO Configuration:** Configure the necessary GPIO pins for the ultrasonic sensor's trigger and echo signals.

## Setup

### Sensor Node (nRF52832 with Ultrasonic Sensor)
1. Configure the ultrasonic sensor to trigger measurements and read echo signals.
2. Implement distance calculation based on the time-of-flight of the ultrasonic pulse.
3. Flash the firmware onto the nRF52832 board.

## How It Works

1. **Data Collection:** The ultrasonic sensor emits a sound wave and measures the time it takes for the echo to return.
2. **Processing:** The nRF52832 calculates the distance to the object based on the speed of sound and the measured time.
3. **Real-Time Display:** The mobile app visualizes the measured distance.

## Usage

1. Power on the nRF52832DK with the ultrasonic sensor connected.
2. View the real-time distance readings on the terminal.

## Troubleshooting

### 1. Incorrect Distance Readings
- Verify the wiring of the ultrasonic sensor to the nRF52832.
- Check the accuracy of the distance calculation logic in the firmware (e.g., speed of sound constant).
- Ensure there are no obstructions interfering with the ultrasonic signal.

Measure distances with precision!
