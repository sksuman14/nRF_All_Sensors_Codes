# GY-485 Lux Sensor Project (RS485)

## Overview

This repository contains code to interface the GY-485 Lux Sensor using RS485 communication protocol with an nRF52832DK board running Zephyr RTOS. The project reads ambient light (lux) values from the sensor and displays the data via a serial interface.

## Features

- Interfaces with GY-485 lux sensor via RS485 (Modbus RTU).
- Reads and decodes lux data.
- Displays lux readings over a serial terminal.
- Compatible with Zephyr RTOS.

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board.
- GY-485 lux sensor.
- TTL to RS485 Converter Board 
- Connection wires and power supply.

## Software Requirements

1. Zephyr RTOS
2. Nordic SDK (if using Nordic boards)
3. An IDE supporting Zephyr development (e.g., VS Code with Zephyr Extension)
4. CMake, west (for building Zephyr projects)

## Prerequisites

1. **Zephyr RTOS**: Ensure Zephyr is installed and configured. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for setup details.
2. **UART Support**: Ensure UART communication is enabled in the Zephyr configuration.
3. **Modbus Protocol Knowledge**: Familiarity with Modbus RTU protocol is helpful for troubleshooting and extending the code.

## Setup

1. **Configure the Build Environment**: Set up your environment for the nRF52832DK board.
2. **Enable UART Support**: In the `prj.conf` file, enable UART communication.
3. **Wire the Sensor**: Connect the GY-485 sensor to the RS485-to-UART converter with hardware auto control, and then connect the converter to the nRF52832DK.
4. **Build the Project**: Compile and flash the firmware to the board.

## Wiring Diagram (Using TTL to RS485 Converter with Auto Control)

| GY-485 Sensor Pin | RS485 Converter Pin | Notes                    |
|-------------------|---------------------|--------------------------|
| A                 | A                   | RS485 Data Line A       |
| B                 | B                   | RS485 Data Line B       |
| VCC               | VCC (5V)            | Power supply to sensor  |
| GND               | GND                 | Common ground           |

**TTL to RS485 Converter to nRF52832DK:**

| Converter Pin | nRF52832DK Pin | Description              |
|---------------|----------------|--------------------------|
| TX           | TX (e.g., P0.06) | UART Transmit to sensor  |
| RX           | RX (e.g., P0.08) | UART Receive from sensor |
| VCC           | 3.3V or 5V      | Match with board logic   |
| GND           | GND             | Common ground            |


## How It Works

1. **Modbus Request**: The board sends a Modbus RTU command to request lux data.
2. **Sensor Response**: The sensor replies with a data packet containing the lux value.
3. **Data Processing**: The response is decoded and lux values are printed via serial.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
Light intensity : 1124
```

## Usage

After flashing the firmware, the board will continuously query the GY-485 lux sensor and print the received lux values on the serial terminal.

## Troubleshooting

### 1. No Data Received

- Check RS485 wiring (A/B lines, termination resistors if needed).
- Verify UART baud rate and parameters match the sensor's specifications.
- Ensure the RS485 converter is powered properly.

### 2. Invalid or Corrupted Data

- Double-check the Modbus command format and sensor address.
- Ensure sensor has stable power (preferably 5V regulated).
- Verify timing between requests to avoid collision or overloading.

### 3. Sensor Not Responding

- Use Modbus tools like Modpoll to test sensor independently.
- Make sure sensor address is correct and not changed from default.
- Check for proper pull-up/pull-down resistors on RS485 lines if required.

