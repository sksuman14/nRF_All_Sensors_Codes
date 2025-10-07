# SHT40 BLE Node

## Project Overview
This project demonstrates the integration of the SHT40 temperature and humidity sensor with a BLE (Bluetooth Low Energy) node using Zephyr RTOS. The node periodically reads data from the SHT40 sensor and advertises the collected temperature and humidity values over BLE every 3 seconds.

The project is suitable for applications in environmental monitoring, IoT devices, and wireless sensor networks.


 ## Features
- SHT40 Sensor Integration: Reads temperature and humidity data.
- BLE Advertising: Advertises sensor data in the BLE advertisement payload.
- Interval: Advertising interval is set to 3 seconds.
- Optimized Power Management: Implements power-saving techniques to extend battery life.
- Custom Manufacturer Data: BLE packets include temperature and humidity data in the manufacturer-specific data field.

## Hardware Requirements
- Development board with BLE support (e.g., nRF52840, nRF52832).
- SHT40 sensor connected via I2C.
- Power supply or battery for the development board.

## Software Requirements
- Zephyr RTOS SDK installed.
- Compatible toolchain for the target hardware (e.g., GCC ARM Embedded).
- west command-line tool for building and flashing the application.

## BLE Advertisement Data Format
The BLE advertisement packet includes the following fields:

- Company Identifier: Custom or default identifier.
- Temperature: Encoded as a 16-bit integer in °C (multiplied by 100 for precision).
- Humidity: Encoded as a 16-bit integer in %RH (multiplied by 100 for precision).


## Usage
1. Power on the device. The board will:
   - Initialize the SHT40 sensor and start reading temperature and humidity data.
   - Advertise the sensor data every 3 seconds via BLE.
2. Monitor BLE Data: Use a BLE scanner app to view the advertisement packets.
3. Optional: Display Data on OLED: Connect an SSD1306 OLED to visualize the sensor data locally.


## Troubleshooting
1. Sensor Not Detected:
  - Verify the I2C wiring and the SHT40 address (default: 0x44).
  -  Ensure I2C is enabled in prj.conf.

2.  BLE Not Advertising:
  - Check that BLE support is enabled in the configuration.
  - Ensure there are no errors in the debug logs (CONFIG_BT_DEBUG_LOG=y).

3.Build Errors:
  - Verify that all dependencies are correctly installed.
  - Ensure your board configuration matches the hardware.

