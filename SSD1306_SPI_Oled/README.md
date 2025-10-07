# SPI OLED Display Project

## Overview

This repository contains code to interface an SPI-based OLED display SSD1306 with an nRF52832DK board running Zephyr RTOS. The project sends data over SPI to the OLED and displays Hello World.

## Features

- Interfaces with SPI OLED displays (e.g. SSD1306 )
- Supports basic text rendering on the OLED
- Compatible with Zephyr RTOS and Nordic nRF boards

## Hardware Requirements

- nRF52832DK or another Zephyr-compatible board
- SPI OLED Display (128x64, SSD1306 )
- Jumper wires

## Software Requirements

- Zephyr RTOS
- Nordic SDK (optional for nRF boards)
- IDE like VS Code with Zephyr Tools

## Prerequisites

1. **Zephyr SDK Setup**: Follow [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)
2. **Enable SPI and Display Driver**: Modify `prj.conf` and devicetree overlays accordingly

## Wiring Guide

| OLED Pin | Description        | nRF52832DK Pin Example |
|----------|--------------------|-------------------------|
| VCC      | Power (3.3V/5V)    | 3.3V                    |
| GND      | Ground             | GND                     |
| SDA      | SPI Data (MOSI)    | P0.23                   |
| SCK      | SPI Clock (SCK)    | P0.25                   |
| RES      | Reset              | P0.19                   |
| DC       | Data/Command       | P0.20                   |
| CS       | Chip Select        | P0.22                   |

> ⚠️ Note: Pin names may vary depending on the OLED model.

## Zephyr Configuration

Add the following to your `prj.conf`:

```ini
CONFIG_SPI=y
CONFIG_DISPLAY=y
CONFIG_SSD1306=y
```

Ensure your board's `.overlay` file includes the SPI and display node configuration.

## How It Works

1. OLED is initialized over SPI using the display driver.
2. The application sends text or image buffer to be displayed.
3. The display updates at a regular interval or based on events.

## Sample Output

```console
*** Booting Zephyr OS build v3.x.x ***
OLED Ready
Displaying: Hello World
```

## Usage Steps

1. Connect the OLED display to the nRF52832DK using the wiring guide.
2. Observe text or graphics rendered on the OLED screen.

## Troubleshooting

### 1. Display Not Turning On
- Ensure power connections and voltage levels are correct.
- Check SPI pin mapping and CS/DC/RES control pins.

### 2. Garbage or No Display Output
- Confirm the display model and use the correct driver (e.g., SSD1306).
- Use a logic analyzer to check SPI signals.

### 3. Partial or Flickering Display
- Verify that delays and initialization sequences are correct.
- Try slowing down SPI clock rate.

---

This README follows the same format used for other Zephyr-based peripheral interface projects.

