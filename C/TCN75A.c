// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCN75A
// This code is designed to work with the TCN75A_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=TCN75A_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TCN75A I2C address is 0x48(72)
	ioctl(file, I2C_SLAVE, 0x48);

	// Select control register1(0x01)
	// 12-bit ADC resolution(0x60)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x60;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x00)
	// temp msb, temp lsb
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
		exit(1);
	}
	else
	{
		// Convert the data
		int temp = (data[0] * 256 + (data[1] & 0xF0)) / 16;
		if(temp > 2047)
		{
			temp -= 4096;
		}
		float cTemp = temp * 0.0625;
		float fTemp = (cTemp * 1.8) + 32;
	
		// Output data to screen
		printf("Temperature in Celsius : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	}
}
