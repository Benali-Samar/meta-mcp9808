# meta-mcp9808
A yocto layer for the driver of the I2C sensor "MCP9808" 

This yocto layer has a linux character driver for the temperature sensor "MCP9808" with its userspace application and it is deployed on the raspberry Pi 3B+ board.


# Steps for loading the driver
1.  Use " $ insmod mcp9808.ko" to load the module into the kernel.
2.  Type " $ echo mcp9808 0x18 > /sys/bus/i2c/devices/i2c-1/new_device" for instantiating the module from userspace.
3.  cd into /usr/bin directorie. 
4.  Run " $ mcpuser".
 
And that's it the sensor sending its data via the driver!
