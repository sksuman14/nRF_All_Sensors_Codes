# Relay Control with Zephyr RTOS on NRF Board

This project demonstrates how to control a relay using the Zephyr RTOS on an nRF52832 Development Kit. The relay is toggled on and off at a fixed interval, with the GPIO pin configured as output. This code can be used for basic relay automation or timing-based toggling.

## Hardware Requirements
- nRF52832 Development Board and nRF52832 Node (e.g., nRF52832 etc.)
- Relay Module connected to the GPIO pin

## Software Requirements
- [Zephyr Project](https://zephyrproject.org) installed and set up
- A compatible compiler and SDK for the NRF board (e.g., nRF Connect SDK)

## Code Overview

The main components of the code are:
- GPIO Configuration: Sets the relay pin as output.
- Relay Control Loop: Continuously toggles the relay on and off with a specified delay.
  


