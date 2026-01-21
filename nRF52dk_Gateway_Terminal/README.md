
---

# NRF52DK BLE Gateway Terminal  

This project demonstrates how to use the **nRF52DK** as a **BLE Gateway Terminal** to collect and display data from nearby BLE devices on a terminal. The gateway scans for BLE devices with a matching unique device ID and logs the received data locally on the UART terminal.  

## Hardware Requirements  

- **NRF52DK (e.g., nRF52832 Development Kit)**  
- BLE-enabled peripheral devices with the unique device ID  
- USB cable for power and UART communication  
- Optional: BLE-enabled mobile device or PC for monitoring  

## Software Requirements  

- [Zephyr Project](https://zephyrproject.org) installed and set up  
- Compatible SDK for the nRF board (e.g., nRF Connect SDK)   

## Features  

- **BLE Scanning**: Detects nearby BLE devices broadcasting the  unique device ID.  
- **Data Logging**: Displays received BLE data, including payloads and signal strength, on a terminal.  
- **Terminal Output**: Logs device-specific information in a structured format.  

## How It Works  

1. **Initialization**:  
   - The NRF52DK initializes BLE in scanning mode to detect nearby devices.  
   - Configures UART for communication with a PC or monitoring device.  

2. **Device Matching**:  
   - The gateway scans for BLE devices broadcasting the unique device ID.  
   - When a matching device is found, the gateway requests and receives data.  

3. **Data Display**:  
   - Logs received BLE data, including the unique device ID, and payload, to a UART terminal.    

## Applications  

- **Local Data Monitoring**: View sensor data from multiple BLE devices in real-time.  
- **BLE Prototyping**: Quickly test BLE communication between devices.  
- **IoT Data Consolidation**: Aggregate sensor data locally for analysis.  
 

## Troubleshooting  

### 1. BLE Devices Not Detected  
- Verify that the BLE devices are broadcasting with the unique device ID.  
- Check that the NRF52DK is within range of the BLE devices.  

### 2. No UART Output  
- Confirm UART pin connections.  
- Check the COM port and baud rate settings (default: 115200).  

---  
