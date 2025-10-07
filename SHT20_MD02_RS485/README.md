# MD02 Temperature and Humidity Sensor Project (RS485)

## Overview

This repository contains code to interface the MD02 Temperature and Humidity Sensor using RS485 Modbus RTU with an nRF52832DK board running Zephyr RTOS. The project reads temperature and humidity values from the sensor and displays the data via a serial terminal.

## Features

- Communicates with the MD02 sensor via RS485 using Modbus RTU.
- Sends Modbus requests and receives temperature and humidity data.
- Parses and displays sensor readings over UART.
- Fully compatible with Zephyr RTOS and Nordic nRF boards.

## Hardware Requirements

- nRF52832DK or compatible Zephyr-supported development board
- MD02 Temperature and Humidity Sensor
- TTL to RS485 Converter Board 
- Jumper wires and power supply (3.3V or 5V depending on setup)

## Software Requirements

- Zephyr RTOS
- Nordic SDK (optional for nRF boards)
- West build system and CMake
- Supported IDE (e.g., VS Code with Zephyr Tools)

## Prerequisites

1. **Zephyr SDK Setup**: Follow [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).
2. **Enable UART**: Ensure UART is enabled in your `prj.conf`.
3. **Modbus Protocol Knowledge**: Understand how Modbus RTU framing and registers work.

## Wiring Guide

### MD02 Sensor to RS485 Converter

| MD02 Pin | RS485 Converter | Description         |
|----------|------------------|---------------------|
| A        | A                | RS485 Bus Line A    |
| B        | B                | RS485 Bus Line B    |
| VCC      | 5V               | Power Input         |
| GND      | GND              | Ground              |

### RS485 Converter to nRF52832DK

| Converter Pin | nRF52832DK Pin | Description               |
|----------------|----------------|---------------------------|
| TX            | TX (e.g., P0.06) | UART TX to send commands |
| RX            | RX (e.g., P0.08) | UART RX to receive data  |
| VCC            | 3.3V/5V         | Match with board voltage |
| GND            | GND             | Common Ground            |


## How It Works

1. The board sends a Modbus command to request temperature and humidity data.
2. The sensor responds with a Modbus frame containing the data.
3. UART driver parses the response and converts it into readable values.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
Temperature: 23.6 °C Humidity: 54.1 %
```

## Usage Steps

1. Connect the MD02 sensor and RS485 converter to the board as per wiring guide.
2. Flash the firmware onto the nRF52832DK.
3. Open a serial terminal at 9600 baud to view temperature and humidity readings.

## Troubleshooting

### 1. No Output
- Check UART TX/RX pin connections.
- Verify Modbus command and slave ID.
- Ensure sensor is powered and RS485 lines are correctly connected.

### 2. Garbage Data
- Make sure baud rate and UART config match sensor settings.
- Check for noise on RS485 lines (use shielded cables if needed).

### 3. Incorrect Sensor Values
- Confirm register addresses are correct.
- Try polling with a Modbus PC tool (e.g., Modpoll) to verify data.

---


