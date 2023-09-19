
SUMMARY = "MCP9808 kernel module driver"
DESCRIPTION = "MCP9808 I2C temperature sensor's driver recipe"
LICENSE = "CLOSED"

# Sources
SRC_URI = "git://github.com/Benali-Samar/MCP9808_LDD.git;protocol=https;branch=main"
SRCREV = "f1532fa8102e5f333c4f3c5d0736b8f977458fff"

# Unpacking
S = "${WORKDIR}/git"

# Heritting kernel module classe
inherit module

# Installing
do_install:append() {
    install -d ${D}/lib/modules/mcp
    install -m 0755 mcp9808.ko ${D}/lib/modules/mcp
}

