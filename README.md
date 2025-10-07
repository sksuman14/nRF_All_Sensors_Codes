# Embedded Systems Projects using nRF52832 Board

This repository contains various experiments and projects for embedded systems using Zephyr RTOS with nRF52832 Development Kit. The projects utilize different sensors, displays, and memory devices to demonstrate their functionality and integration.

## Table of Contents

- [accelerometerOnMobileAPP](#accelerometer-on-mobileapp)
- [adcRead](#adc-Read)
- [bme680Sensor](#bme680sensor)
- [buzzer](#buzzer)
- [co2](#CO2-experiment)
- [grove16x2Lcd](#grove-16-2-lcd)
- [gy485LuxRS485](#gy485-Lux-RS485)
- [i2cScanner](#i2c-scanner)
- [ledBlink](#blinky-experiment)
- [ledWithRemote](#led-with-remote)
- [lis3dhSensor](#lis3dh-sensor)
- [magnetometerTlv493d](#magnetometer)
- [nRF52dkGatewayTerminal](#nRF52dk-gateway-terminal)
- [oledDisplay](#oled-display)
- [oledWithSensor](#oled-with-sensor)
- [relay](#relay)
- [ringWithRemote](#ring-with-remote)
- [servoMotor](#servo-Motor)
- [sht20md02RS485](#sht20-md02-RS485)
- [shtOnMobileAPP](#sht40-on-mobileapp)
- [spiFlash](#spi-flash-memory)
- [spiOled](#spi-oled)
- [stepMotor28BYJ-48](#step-Motor-28BYJ-48)
- [temperatureSensors](#temperature-sensor)
- [tftDisplay](#tft-display)
- [tofVl5310x](#tof-vl53l0x)
- [uvLtr390](#uv-ltr390)
- [vcnlLuxSensor](#vcnl-lux-sensor)
  
## Accelerometer ON MobileAPP
This sample application periodically measures Three axis DATA and send through BLE on the Mobile APP.

## Adc Read
This repository contains code to interface with the Analog-to-Digital Converter (ADC) on an nRF52832DK board using Zephyr RTOS. The project reads analog voltage values and displays the results via a serial interface.

## Bme680Sensor
This sample application periodically measures 4 parameter of the sensor temperature, presure, hummidity and Gas .

## Buzzer
This project is an audio signaling device that produces a buzzing or beeping sound when an external voltage is applied.

## CO2 Experiment
This sample application demonstrate the configurations of the MH-Z19B CO2 sensor, then periodically reads CO2 data from the sensor.

## Grove 16 2 LCD
This sample application demonstrate the configurations of the Grove 16*2 LCD and print the Hello world on the display.

## GY485 Lux RS485
This repository contains code to interface the GY-485 Lux Sensor using RS485 communication protocol with an nRF52832DK board running Zephyr RTOS. The project reads ambient light (lux) values from the sensor and displays the data via a serial interface.

## I2C Scanner
A simple I2C scanner to detect and list all I2C devices connected to the bus.

## Blinky Experiment
A simple "blinky" experiment to demonstrate GPIO control. This project toggles an LED on and off at a defined interval, showcasing basic embedded programming principles.

## LED with remote
This project demonstrates how to control a Board LED using the remote . The remote works on BLE for communication and give the value to the controller to on the the LED on the board.

## LIS3DH Sensor
This project integrates the LIS3DH accelerometer sensor to read acceleration data. It utilizes SPI communication to interface with the sensor and processes the accelerometer data for further applications.

## Magnetometer
This project integrates the (TLV493D) Magnetometer sensor to read magnetic field. 

## NRF52DK Gateway Terminal
A simple application demonstrating Bluetooth Low Energy Observer role functionality. The application will periodically scan for devices nearby. If any found, prints the address of the device, the RSSI value, the Advertising type, and the Advertising data length to the console.

## OLED Display
This project utilizes an SSD1306 OLED display to show dynamic text and sensor readings. Static text labels are used alongside changing numerical values for improved readability.

## OLED With Sensor
This project demonstrates how to interface an SSD1306 OLED display with the LIS3DH accelerometer sensor, VCNL4040 proximity and ambient light sensor and the SHT40 temperature and humidity sensor. The OLED display will show readings from these sensors in real-time.

## RELAY
This project demonstrates how to control a relay using the Zephyr RTOS on an nRF52832 Development Kit. The relay is toggled on and off at a fixed interval, with the GPIO pin configured as output. This code can be used for basic relay automation or timing-based toggling.

## Ring with remote
This project demonstrates how to control a ring LED using the remote . The remote works on BLE for communication and give the value to the controller to on the the ring LED.

## Servo Motor
This repository contains code to interface a servo motor with an nRF52832DK board using Zephyr RTOS. The project controls the servo motor position using PWM signals and allows for angle adjustments.

## SHT20 MD02 RS485
This repository contains code to interface the MD02 Temperature and Humidity Sensor using RS485 Modbus RTU with an nRF52832DK board running Zephyr RTOS. The project reads temperature and humidity values from the sensor and displays the data via a serial terminal.

## SHT40 ON MobileAPP
This sample application periodically measures the temperature, humidity DATA and send through BLE on the Mobile APP .

## SPI Flash Memory
This project demonstrates reading and writing operations on SPI flash memory, specifically the W25R64JV. The code is optimized for handling multiple SPI flash memories and ensures efficient memory management.

## SPI Oled
This repository contains code to interface an SPI-based OLED display SSD1306 with an nRF52832DK board running Zephyr RTOS. The project sends data over SPI to the OLED and displays Hello World.

## Step Motor 28BYJ-48
This repository contains code to interface and control a 28BYJ-48 stepper motor using an nRF52832DK board with Zephyr RTOS. The project uses GPIO and PWM signals to drive the motor through a ULN2003AN driver board.

## Temperature Sensor
Integration of a temperature sensor to read ambient temperature values. This project can be modified to display the temperature readings on the TFT or OLED display.

## TFT Display
This project demonstrates the use of a TFT display with the ST7735 driver. The display is utilized to show various graphical outputs and information from different sensors.

## TOF VL53L0X
This sample periodically measures distance between vl53l0x sensor and target. The result is displayed on the console. It also shows how we can use the vl53l0x as a proximity sensor.

## UV ltr390
This project demonstrates to Integration of a UV LTR390 sensor to the nrf52dk board to read the UV Index value.

## VCNL Lux Sensor
Integration of the VCNL4040 lux sensor to measure ambient light levels. This project includes handling sensor resets upon connection and reading lux values for display or logging.

## Getting Started
### Prerequisites

- [Zephyr RTOS](https://zephyrproject.org/)
- [nRF52832 Development Board](https://www.nordicsemi.com/en/design-center/nrf52832)
- Required libraries for sensor and display communication

### Installation

