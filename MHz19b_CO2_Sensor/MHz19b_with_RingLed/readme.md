

# CO2 Sensor with Ring LED Using Zephyr RTOS

This project demonstrates how to measure CO2 levels using a **MHZ19B CO2 sensor** and display different colors on a **Ring LED** strip based on the CO2 readings. The application runs on an **nRF52832 Development Kit (DK)** with the Zephyr RTOS. The project is ideal for visualizing air quality based on CO2 concentration through dynamic LED color changes.

## Hardware Requirements

- **NRF-based Development Board** (e.g., nRF52832)
- **MHZ19B CO2 Sensor**
- **Ring LED Strip** (WS2812 or compatible RGB LEDs)
- Necessary wiring and connectors

## Software Requirements

- [Zephyr Project](https://zephyrproject.org) installed and set up
- A compatible compiler and SDK for the NRF board (e.g., nRF Connect SDK)
- Libraries for sensor interfacing and LED control (`led_strip.h`, `sensor.h`)

Requirements
************

The baudrate of the UART must be configured to 9600, and must support
interrupt driven API.

## Code Overview

### Functionality

The program:
- Continuously reads CO2 levels from the MHZ19B sensor via UART.
- Displays the CO2 concentration on a ring LED strip, changing the color based on the concentration range.

### Key Components

1. **Sensor Reading**:
   - The `gas_data()` function fetches CO2 data from the MHZ19B sensor.
   - `sensor_sample_fetch()` and `sensor_channel_get()` are used to retrieve the sensor values.

2. **Color Display**:
   - Based on CO2 levels, different colors are shown on the ring LED.
   - Colors change dynamically using the `led_strip_update_rgb()` function.

### CO2 Level Color Mapping

- **Red** (CO2 >= 4000 ppm)
- **Blue** (3000 <= CO2 < 4000 ppm)
- **Magenta** (2000 <= CO2 < 3000 ppm)
- **Yellow** (1000 <= CO2 < 2000 ppm)
- **Green** (400 <= CO2 < 1000 ppm)

## Connections

### CO2 Sensor (MHZ19B) Connections

| Sensor Pin | Microcontroller Pin |
|------------|---------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| UART TX    | RX Pin (UART)       |
| UART RX    | TX Pin (UART)       |

### Ring LED Connections

| Ring LED Pin | Microcontroller Pin |
|--------------|---------------------|
| VDD          | 5V (External)       |
| GND          | GND                 |
| DI           | P0.23               |

## How It Works

1. **Initialization**:
   - The sensor is configured, and its attributes (e.g., full scale, ABC mode) are set.

2. **Data Reading**:
   - The `gas_data()` function continuously reads data and calls `data_fetch()` to update LED colors based on CO2 levels.

3. **LED Color Control**:
   - The ring LED color changes to represent CO2 levels in different ranges using the `led_strip_update_rgb()` function.

## Troubleshooting

### 1. No LED Color Change

- Verify LED connections (VDD, GND, DI).
- Ensure the correct pin mapping in the code.

### 2. Incorrect CO2 Readings

- Check the MHZ19B sensor connections.
- Confirm proper UART configuration.

  
