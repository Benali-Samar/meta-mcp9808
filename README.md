# meta-mcp9808 layer
A yocto layer for the driver of the I2C temperature sensor "MCP9808" 

This yocto layer has a linux character driver for the temperature sensor "MCP9808" 
with its userspace applications and it is deployed on the raspberry Pi 3B+ board.




Dependencies
============

- Meta-raspberry : "https://github.com/agherzan/meta-raspberrypi"


Table of Contents
=================

  I. Yocto layer "meta-mcp9808" containing four recipes. 

    1. Recipes kernel
    2. Recipes user
    3. Recipes BSP
    4. Recipes core


 II. Recipes kernel:

  The main recipe of the driver it has the character driver source code in C language with its Makefile that contain compilation scenarios.

  The driver should contain all the entry/remove kernel routines, and as its a char driver it should also contain the open/close file routines
  with all character drivers specifications like major and minor number allocation and creation of sysfs access ...
  The driver also should be integrated in the device tree source so it has some instructions for device tree compatible and ID.

  III. Recipes user:

  The driver will just send data to its file under "/dev", the user reciepe contain a simple C++ application source code that will open this file  of the driver and display it on the terminal screen, also this reciepe contain the CMakelists that will compile this application.

  IV. Recipes BSP: 

   The BSP recipe contain the Device tree overlay and the raspberry "bootconfig" append file.

   - Device Tree Overlay:

     This is like a way to add a node of a device to the Device tree source "DTS" that will be applied to the original DTS in the boot time, so our I2C sensor node will be integrated under the I2C bus node.

   - RPI append Bootfiles config :

     This append file will enable the Device tree overlay to the config file of the raspberry pi.

   VII. Recipes core: 

   The core recipes will host the image recipe and the package groupe recipe:

   - Package groups:

     This recipe file will call the packages needs in the runtime such as the driver and the DT overlay and the driver application.

  - Image recipe :

     The image recipe will call the other recipes in one image file that will be deployed on the board.

Layer tree:
============     

	   .
	├── conf
	│   └── layer.conf
	├── COPYING.MIT
	├── README.md
	├── recipes-bsp
	│   ├── bootfiles
	│   │   └── rpi-config_%.bbappend
	│   └── mcp
	│       ├── files
	│       └── mcp-overlay.bb
	├── recipes-core
	│   ├── images
	│   │   ├── mcp-image-basic_0.1.bb
	│   │   ├── mcp-image-basic_0.2.bb
	│   │   └── mcp-image-basic_0.3.bb
	│   └── packagegroups
	│       └── packagegroup-mcp.bb
	├── recipes-example
	│   └── example
	│       └── example_0.1.bb
	├── recipes-kernel
	│   └── MCP9808_driver
	│       └── mcp-driver_0.1.bb
	└── recipes-user
	    └── mcpuser
		├── mcpapp_0.1.bb
		├── mcpapp_0.2.bb
		├── mcpapp_0.3.bb
		└── mcpapp.inc




TO-DO:
============

    [X] Wayland distro (distro features appended to the poky distro)
    [ ] Integrate the QT app
    [ ] Make a skeleton for integration
    [ ] Add some demo pics
