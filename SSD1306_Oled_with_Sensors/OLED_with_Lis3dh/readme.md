# OLED Screen and LIS3DH Sensor Project  

This repository contains code for integrating and using an OLED screen to display accelerometer data from the LIS3DH sensor using Zephyr RTOS on an embedded platform.  

---  

## Features  

- Reads accelerometer data (X, Y, Z axes) from the LIS3DH sensor.  
- Communicates via the I2C or SPI interface (configurable).  
- Displays sensor data on a connected OLED screen in real time.  
- Configurable update rates and thresholds for sensor readings.  
- Compatible with Zephyr RTOS.  

---  

## Hardware Requirements  

- **LIS3DH Sensor** (3-axis accelerometer with I2C/SPI communication).  
- **OLED Screen** (e.g., SSD1306 or compatible).  
- **Microcontroller/Board**: nRF52832 (or any other Zephyr-compatible platform with I2C/SPI support).  

---  

## Software Requirements  

- **Zephyr RTOS**  
- **Nordic SDK** (if using Nordic boards)  
- **IDE**: VS Code with Zephyr Extension or similar  
- **Build Tools**: CMake, west (for building Zephyr projects)  

---  

## Prerequisites  

- **Zephyr RTOS**: Ensure you have Zephyr RTOS installed and properly set up. Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org) for installation instructions.  
- **CMake and west**: Required for building the Zephyr project.  
- **LIS3DH Sensor Driver**: Ensure the driver is included in the device tree and enabled in the configuration.  
- **OLED Display Driver**: Enable the SSD1306 driver (or the driver for your specific OLED) in the configuration.  

---  

## Setup  

1. **Configure the Build Environment**  
   - Set up the environment for your board (e.g., nRF52832).  

2. **Enable LIS3DH and OLED Support**  
   - Update the `prj.conf` file to enable I2C/SPI, sensor, and OLED display support.  

3. **Building the Project**  
   - Build the project using Zephyr's build tools (`west build`).  

4. **Flashing the Firmware**  
   - Flash the firmware onto your board after a successful build using `west flash`.  

---  

## Pin Configuration  

| LIS3DH/OLED Pin  | Microcontroller Pin |  
|------------------|----------------------|  
| VCC              | 3.3V                |  
| GND              | GND                 |  
| SDA              | I2C Data Line       |  
| SCL              | I2C Clock Line      |  

---  
---  

## OLED Display Example  

The OLED screen will display:  
```plaintext  
Accelerometer:  
X: -0.15  
Y: 0.00  
Z: 9.50 
```  

---  

## Usage  

Once the board is flashed, the application will:  

1. Initialize the LIS3DH sensor and OLED display.  
2. Continuously read X, Y, and Z-axis accelerometer data.  
3. Display the accelerometer data on the OLED screen.  
4. Log the sensor data to the serial console for debugging or monitoring.  

---  

## Troubleshooting  

### 1. Sensor or OLED Not Detected  
- **Solution**: Ensure the connections are correct.  
- **Solution**: Verify the I2C/SPI addresses for the LIS3DH and OLED display.  
- **Solution**: Ensure I2C/SPI is enabled in the configuration (`prj.conf`).  

### 2. Build Issues  
- **Solution**: Double-check the configuration and device tree bindings.  
- **Solution**: Verify the environment setup as per Zephyr project guidelines.  

### 3. OLED Display Issues  
- **Solution**: Ensure the display driver (e.g., SSD1306) is enabled and configured correctly.  
- **Solution**: Verify the resolution matches your display (e.g., 128x64 or 128x32).  

### 4. Incorrect Accelerometer Readings  
- **Solution**: Check the LIS3DH orientation and sensitivity configuration.  
- **Solution**: Verify the LIS3DH initialization parameters (e.g., range, data rate).  

### 5. High Power Consumption  
- **Solution**: Use Zephyr's power management features to enter low-power mode when idle.  
- **Solution**: Reduce the data update rate if permissible.  

