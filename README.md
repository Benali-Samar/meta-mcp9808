# meta-mcp9808
A yocto layer for the driver of the I2C sensor "MCP9808" 

This yocto layer has a linux character driver for the temperature sensor "MCP9808" 
with its userspace application and it is deployed on the raspberry Pi 3B+ board.

Add this meta-layer to your yocto image, by cloning this and adding the path of this meta to the "bblayers.conf" file, focus on adapting it to your needs.

# Steps for loading the driver while the yocto image is runing on the target:
1.  Use " $ insmod /lib/modules/(kernel version)/extra/mcp9808.ko" to load the module into the kernel.
2.  Type " $ echo mcp9808 0x18 > /sys/bus/i2c/devices/i2c-1/new_device" for instantiating the driver module from userspace.
3.  Run " $ mcpuser".
 
And that's it the sensor sending its data via the driver!
