# Relay Control with Zephyr RTOS on nRF Node

This project demonstrates how to control a relay using the Zephyr RTOS on an nRF52832 Node. The relay is toggled on and off at a fixed interval, with the GPIO pin configured as output. This code can be used for basic relay automation or timing-based toggling.

## Hardware Requirements
- nRF Node
- Relay Module connected to the GPIO pin

## Software Requirements
- [Zephyr Project](https://zephyrproject.org) installed and set up
- A compatible compiler and SDK for the NRF board (e.g., nRF Connect SDK)

## Code Overview

The main components of the code are:
- GPIO Configuration: Sets the relay pin as output.
- Relay Control Loop: Continuously toggles the relay on and off with a specified delay.
  


## Relay Pin Connections

| Relay Pin | Node Pin        |
|-----------|-----------------|
| VCC       | 3.3V            |
| GND       | GND             |
| IN1       | P0.27 (SCL pin) |

## How It Works
1. GPIO Initialization: The relay GPIO pin is configured as an output during initialization.
2. Toggling Loop: A loop toggles the relay state at the interval defined in TOGGLE_INTERVAL_MS.
3. Power Management: Efficient delay handling ensures minimal power consumption when idle.

## Troubleshooting
1. Relay Not Toggling:
   
 - Verify the relay's power supply (3.3V or 5V as required).
 -  Check the GPIO pin configuration in the main.c file.
 
2. Build/Flash Issues:
   
 - Confirm Zephyr and nRF Connect SDK are correctly installed.
 - Ensure the correct board target is specified in the build command (west build -b nrf52832dk_nrf52832).

