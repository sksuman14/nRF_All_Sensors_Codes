# 🌫️ SEN66 BLE Environmental Sensor (Zephyr)

This project demonstrates how to interface the **SEN66 environmental sensor** with a **Zephyr RTOS-based device** and broadcast real-time air quality data using **Bluetooth Low Energy (BLE) Extended Advertising**.

---

##  Features

-  Reads environmental data via **I2C**
-  Measures:
  - PM1.0, PM2.5, PM4.0, PM10
  - Temperature
  - Relative Humidity
  - VOC Index
  - NOx Index
  - CO₂ concentration
-  Broadcasts data using **BLE Extended Advertising**
-  Periodic **fan cleaning (every 24 hours)**
-  Serial debug output via `printk`

---

##  Hardware Requirements

- Zephyr-supported board (e.g., nRF52 / nRF53)
- SEN66 Sensor (I2C interface)
- Proper I2C wiring:
  - SDA
  - SCL
  - GND
  - VCC

---

##  Software Requirements

- Zephyr RTOS
- West build system
- CMake & Ninja
- Bluetooth support enabled in Zephyr

---

##  Configuration

### I2C Configuration

The sensor is connected using device tree:
## ⚙️ Setup

### Configure the Build Environment
Set up the development environment for your **nRF52832DK board**.

### Enable I2C and BLE Support
In the `prj.conf` file, ensure that I2C for the Sen66 and BLE are enabled.

### Build the Project
Build and flash the code for the **nRF52832DK**.

### Pairing with Mobile App
Pair the device with a **BLE-capable mobile app** that can receive and display the Sen66 data.

---

### Data Transmission over BLE
The board transmits accelerometer data to a mobile app over BLE.

### Real-Time Display on Mobile App
The mobile app displays the accelerometer readings in real time.

---

##  Output on Mobile App

The Sen66 data is displayed on the mobile application after successful pairing.

---

##  Usage

Once the board is flashed:

- Power on the nRF52832DK  
- Open a BLE-compatible app on your mobile device  
- Pair the board with the app  
- Start receiving live Sen66 data  

---

##  Troubleshooting

### 1. BLE Connection Issues
- Confirm that both devices are within BLE range and powered  
- Check that BLE pairing was successful  
- Ensure BLE settings are correctly configured in `prj.conf`  

### 2. Data Not Displaying on Mobile App
- Verify the Sen66 is connected properly  
- Check device tree configuration  
- Confirm the mobile app is set up to receive data  
- Enable GPS and Bluetooth on the mobile device  
```c
#define I2C_NODE DT_NODELABEL(sen66)
