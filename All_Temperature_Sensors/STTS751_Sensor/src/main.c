#include <zephyr/zephyr.h>                                // header files for the project
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>

#define SLEEP_TIME_MS   1000   // Define the sleep time in milliseconds
#define stts751TempHighReg            0x00  // Define the register address for temperature high byte
#define stts751TempLowReg           0x02  // Define the register address for temperature low byte
#define stts751ConfigReg               0x03  // Define the register address for configuration

#define I2C_NODE DT_NODELABEL(mysensor)   // Define the I2C node label as 'mysensor'

void main(void)   // main function
{

	int ret;  // Declare a variable named 'ret' of type 'int'

	static const struct i2c_dt_spec devI2c = I2C_DT_SPEC_GET(I2C_NODE);  // Get the I2C device specification from the device tree using the specified node label
	if (!device_is_ready(devI2c.bus)) {                   // Check if the I2C bus associated with the device is not ready
		printk("I2C bus %s is not ready!\n\r",devI2c.bus->name);   // Print an error message indicating that the I2C bus is not ready
		return;            // Return from the function
	} 


	uint8_t buffer[2] = {stts751ConfigReg,0x8C};  // Define a buffer containing data to be written to the I2C device
	ret = i2c_write_dt(&devI2c, buffer, sizeof(buffer));   // Write data to the I2C device using the device-specific I2C descriptor and the buffer
	if(ret != 0){    // Check if the return value from the I2C write operation is not equal to 0 (indicating an error)
		printk("Failed to write to I2C device address %x at Reg. %x \n", devI2c.addr,buffer[0]);  // Print an error message indicating the failure to write to the I2C device 
		return;   // Return from the function
	}

	while (1) {    // infinite loop

		uint8_t temp_reading[2]= {0};    // Define an array to store temperature readings
		uint8_t sensor_regs[2] ={stts751TempLowReg,stts751TempHighReg};   // Define an array containing the sensor register addresses for temperature readings
		ret = i2c_write_read_dt(&devI2c,&sensor_regs[0],1,&temp_reading[0],1);  // Perform an I2C write-read operation to read temperature data from the sensor registers
		if(ret != 0){                                // Check if the return value from the I2C operation is not equal to 0 (indicating an error)
			printk("Failed to write/read I2C device address %x at Reg. %x \r\n", devI2c.addr,sensor_regs[0]);  // Print an error message indicating the failure to write/read from the I2C device
		} 
		ret = i2c_write_read_dt(&devI2c,&sensor_regs[1],1,&temp_reading[1],1);  // Perform another I2C write-read operation to read temperature data from the sensor registers
		if(ret != 0){            // Check if the return value from the I2C operation is not equal to 0 (indicating an error)
			printk("Failed to write/read I2C device address %x at Reg. %x \r\n", devI2c.addr,sensor_regs[1]);  // Print an error message indicating the failure to write/read from the I2C device
		}


		int temp = ((int)temp_reading[1] * 256 + ((int)temp_reading[0] & 0xF0)) / 16;   // Calculate the temperature value from the raw temperature readings
		if(temp > 2047)  // Check if the temperature value is greater than 2047 (indicating a negative temperature)
		{
			temp -= 4096;  // Adjust the temperature value for negative temperatures
		}

		
		double cTemp = temp * 0.0625;   // Convert the temperature value to Celsius and Fahrenheit
		double fTemp = cTemp * 1.8 + 32; 

		
		printk("Temperature in Celsius : %.2f °C  Temperature in Fahrenheit : %.2f F\n", cTemp, fTemp);  // Print the temperature in Celsius and Fahrenheit
		k_msleep(SLEEP_TIME_MS);   // Delay for a specified time before the next iteration
	}
}