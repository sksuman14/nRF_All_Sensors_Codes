# SEN66 Environmental Sensor (I2C - Zephyr)

This project demonstrates how to interface the **SEN66 environmental sensor** using **I2C** with a Zephyr-based device and read real-time air quality data.

---

## Features

- I2C communication with SEN66 sensor
- Measures:
  - PM1.0, PM2.5, PM4.0, PM10
  - Temperature
  - Relative Humidity
  - VOC Index
  - NOx Index
  - CO₂ concentration
- Automatic **fan cleaning every 24 hours**
- Serial output using `printk`

---

## Hardware Requirements

- Zephyr-supported board (nRF52 / nRF53 recommended)
- SEN66 sensor (I2C interface)
- Proper wiring:
  - SDA
  - SCL
  - VCC
  - GND

---

## Software Requirements

- Zephyr RTOS
- West build system
- CMake & Ninja

---

## Setup

### Configure the Build Environment
Set up the Zephyr development environment for your board.

### Enable I2C Support
In the `prj.conf` file:

```conf
CONFIG_I2C=y
