SUMMARY = "MCP9808 kernel module driver"
LICENSE = "CLOSED"


inherit module

SRC_URI = "file://Makefile \
           file://mcp9808.c \
          "

S = "${WORKDIR}"

RPROVIDES_${PN} += "kernel-module-driver"
