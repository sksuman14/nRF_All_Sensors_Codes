# Blinky Application with Zephyr RTOS


This repository contains a simple Blinky application for Zephyr RTOS. The program toggles an LED on and off at regular intervals, demonstrating how to work with GPIO (General Purpose Input/Output) and timers in Zephyr.

## Features:
 

- Onboard LED Control: Simple blinking of the onboard LED.
- External LED Control: Control of an external LED connected to a GPIO pin.
- Button with Onboard LED: Toggles the onboard LED when the button is pressed.
- Button with External LED: Toggles an external LED using a button.
- Ring LED Control: Lighting up an LED ring connected to the board.
- Breathing Effect on Ring LED: Smooth fade-in and fade-out effect on the LED ring.

##  Hardware Requirements:
 

- Microcontroller/Board: Any board supported by Zephyr RTOS with at least one GPIO pin connected to an LED (e.g., NRF52832, NRF52840 etc).
- LED: Typically connected to a GPIO pin (or onboard LED).
- External LED
- Ring LED
- Necessary wiring and resistors (if required)

## Prerequisites:
 

- Zephyr RTOS: Install Zephyr RTOS and set up your environment. You can follow the official Guide to get started.[Nordic DevAcademy](https://academy.nordicsemi.com/)
 

- CMake: Required for building Zephyr applications.

# Setup:
                                                                                                              
## 1. Device Tree Configuration:
                                                                                                                 
                                                                                                              
Ensure that the correct GPIO pin is mapped to the LED in your board's device tree (.dts or .overlay file). For example, on the NRF52832, the onboard LED is usually mapped like this:

## Usage:
 
Once the firmware is flashed, the LED connected to the board (or onboard LED) will blink at the specified interval.

## Pin Configuration:
 
Ensure the pin connected to the LED is properly configured in the device tree file (.dts or .overlay). For example, for a board with an LED on pin 17.


## Troubleshooting
 
## 1. LED Not Blinking:
 
- Ensure that the correct GPIO pin is configured in the device tree.
- Check the physical connection to the LED.
- Verify that your board's LEDs are functional.
  
## 2. Build Errors:
 
- Ensure that the necessary GPIO configuration is enabled in the prj.conf file.
- Double-check that you have selected the correct board when building the project.














