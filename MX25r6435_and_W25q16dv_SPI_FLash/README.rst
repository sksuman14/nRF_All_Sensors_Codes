.. _spi-nor-sample:


W25Q16DV SPI Flash Memory Project
#################################

This repository contains code for integrating and using the W25Q16DV SPI flash memory chip with Zephyr RTOS on an embedded platform.
***********************************************************************************************************************************

Features:
*********

- Read, write, and erase operations on the W25Q16DV SPI flash memory.
- Uses the SPI interface for communication.
- Supports multiple operations such as sector erases, page programming, and reading memory.
- Compatible with Zephyr RTOS.

Hardware Requirements:
**********************
- W25Q16DV SPI Flash Memory.
- Microcontroller/Board: NRF52832 (or any other Zephyr-compatible platform with SPI support).
- Optional: Logic analyzer for debugging SPI communication.

Prerequisites:
**************

1. Zephyr RTOS: Ensure you have Zephyr RTOS installed and properly set up. You can refer to the Zephyr Getting Started Guide for installation and setup instructions.
2. CMake: Used for building the Zephyr project.
3. SPI Flash Driver: Ensure the W25Q16DV driver is included and enabled in the device tree configuration.
4. Register: Ensure that if it require an pull up register.

Setup:
******

1. Wiring and Pin Connections

W25Q16DV Pin	  Microcontroller Pin

-VCC	          3.3V
-GND	          GND
-CS	            SPI CS
-MISO	          SPI MISO
-MOSI           SPI MOSI
-CLK	          SPI CLK

2. Enable SPI and Flash Memory in Zephyr
3. Device Tree Configuration
4. Build and Flash the Project

Sample Output
=============

.. code-block:: console

   *** Booting Zephyr OS build zephyr-v2.3.0-2142-gca01d2e1d748  ***

    SPI flash testing
   ==========================

   Test 1: Flash erase
   Flash erase succeeded!

   Test 2: Flash write
   Attempting to write 4 bytes
   Data read matches data written. Good!


Usage:
******

Once the board is flashed, the application will start reading temperature and humidity data from the SHT40 sensor at regular intervals and print the results over the serial console.

Optional: Display Additional Flash Operations:
*********************************************

You can extend this project to include additional flash operations such as bulk erase, deep power down mode, or reading the flash memory ID.


Troubleshooting:
****************

1. Sensor Not Detected:

- Ensure the wiring is correct, especially the CS, MOSI, MISO, and CLK connections.
- Check the device tree bindings for SPI and flash memory.

2. Build Issues:

- Verify that SPI and flash memory support are enabled in prj.conf and that the device tree is correctly configured.
- Ensure the environment is correctly set up as per Zephyr project guidelines.

3. Read/Write Failures:

- Double-check the memory addresses and offsets being used.
- Ensure the flash memory sector is erased before writing new data.


