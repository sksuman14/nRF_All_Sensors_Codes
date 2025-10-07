# Extended Accelerometer

This project demonstrates the use of **BLE Periodic Advertising** to transmit accelerometer data in a power-efficient manner using the nRF52DK. The node reads accelerometer data for a fixed duration, broadcasts it via BLE, and then enters a low-power sleep mode before repeating the process.  

## Features  

- **Accelerometer Data Collection**: Periodically reads data from an onboard or external accelerometer.  
- **BLE Periodic Advertising**: Publishes the collected data via BLE advertisements.  
- **Power Management**: Implements a sleep cycle to conserve energy.  

## Functionality  

1. The node initializes and configures the accelerometer and BLE stack.  
2. **Data Collection Phase**: Reads accelerometer data continuously for **40 seconds**.  
3. **Publishing Phase**: Transmits the collected data using BLE periodic advertising.  
4. **Sleep Phase**: Enters low-power mode for **20 seconds** before restarting the cycle.  

## Hardware Requirements  

- nRF52832 Development Kit  
- Accelerometer sensor module (LIS3DH.)   

## Software Requirements  

- [Zephyr Project](https://zephyrproject.org) installed and set up  
- Compatible SDK for the nRF board (e.g., nRF Connect SDK)   

## How It Works  

### Data Collection  
- The node collects data from the accelerometer sensor for **40 seconds**.  
- Example data includes X, Y, Z axis values or derived parameters like tilt and vibration.  

### Data Broadcasting  
- After collecting data, it uses BLE periodic advertising to broadcast the data.  
- The BLE advertisement contains sensor readings and other relevant information.  

### Sleep Mode  
- The node enters a low-power sleep mode for **20 seconds**.  
- This reduces power consumption and extends battery life.  

## Output 

```plaintext  
Set Extended Advertising Data...
Start Extended Advertising...done.
Stopped advertising..!!
Entering sleep mode..!!
```  

## Applications  

- **Motion Monitoring**: Track movement and orientation for IoT or fitness devices.  
- **Vibration Analysis**: Detect unusual vibrations for machinery or structural health monitoring.  
- **Periodic Data Transmission**: Send data intermittently to save power in IoT applications.  

## Power Optimization  

- Uses Zephyr's low-power APIs to enter sleep mode during idle periods.  
- BLE Periodic Advertising ensures efficient and reliable data transmission.  

## Limitations  

- BLE range depends on environmental conditions; ensure devices are in close proximity.  
- Sleep intervals and data transmission durations are fixed but can be optimized based on application requirements.  
 
## Note :
This setup is configured for Development Board as default to configure or Flash in node we need to uncomment those commented line(by presseing Ctrl+/ ) in nRF52832_52832.overlay file then flash in Node.
