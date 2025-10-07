# 1.8" TFT Display with Zephyr RTOS
 

This repository contains an example project for controlling a 1.8" TFT display using the ST7735 driver over SPI with Zephyr RTOS.

## Features:
 
- Interface with a 1.8" TFT display using the ST7735 driver.
- Draw basic shapes, text, and images on the display.
- Example code to initialize the display and perform SPI communication.

## Hardware Requirements:
 
- 1.8" TFT Display: Using the ST7735 controller.
- Microcontroller/Board: Compatible with Zephyr RTOS and SPI (e.g., NRF52832, STM32).
- SPI connections to the TFT display:
    - MOSI (Master Out Slave In)
    - SCK (Serial Clock)
    - CS (Chip Select)
    - DC (Data/Command)
    - RST (Reset)

## Prerequisites:
 
1. Zephyr RTOS: Install Zephyr RTOS and set up your environment. You can follow the official Guide to get started.[Nordic DevAcademy](https://academy.nordicsemi.com/)
2. CMake: Needed for building the project.


##  Setup:
 
## 1. Device Tree Configuration:
 
Ensure that the SPI peripheral and the TFT display are properly configured in your board’s device tree (.dts or .overlay)

## 2. Configure the Build Environment:
 
Set up the environment for your specific board. For example, to build for an NRF52832 board.

## 3. Enable Display and SPI Support:
 
Ensure the Display configuration options are enabled in the prj.conf file.  

## 4. Building the Project:
 

Build the project.

## 5. Flashing the Firmware:
 

Once the project is successfully built, you can flash the firmware onto your device:

## Usage:
 
After flashing the firmware, the program will initialize the display and begin drawing on it. Example shapes, text, and other visuals should appear on the display.

## Device Tree and Overlay:
 
Ensure the st7735 device is properly set up in the device tree to match the pins used for your board.

## Pin Connections:
 
Connect the TFT display to the appropriate pins on your board as per the device tree configuration:

## W25Q16DV Flash Memory Pin Connections

| Display Pin | Microcontroller Pin |
|--------------|---------------------|
| VCC          | 3.3V               |
| GND          | GND                |
| CS           | SPI CS             |
| AO           | Data Pin           |
| RST          | Reset Pin          |
| SDA          | SPI MOSI           |
| SCK          | SPI CLK            |



## Troubleshooting:
 
1. No Display Output:
- Check the physical connections of the TFT display.
- Verify the SPI bus is correctly configured and initialized.
- Make sure the device tree overlay matches your hardware setup.

2. Build Errors:
- Ensure you have enabled SPI and display support in the prj.conf file.
- Verify your Zephyr environment is correctly set up.












