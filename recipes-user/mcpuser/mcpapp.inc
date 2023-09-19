
SUMMARY = "A simple user space application for opening the driver file"
DESCRIPTION = "${SUMMARY}"

# No license specified
LICENSE = "CLOSED"

# Sources
SRC_URI = "git://github.com/Benali-Samar/MCP_apps.git;protocol=https;branch=main"
SRCREV = "1eff717044167a908c483233abe318069d71e733"

# Unpacking
S ="${WORKDIR}/git"

# Heriting cmake.bbclass as cmakelists is used in this example
inherit cmake

# Packing
FILES_${PN} += "${bindir}/MCP_App"


