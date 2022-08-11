SUMMARY = "a simple user space application for opening the driver file"
LICENSE = "CLOSED"


SRC_URI = "file://CMakeLists.txt\
           file://mcpuser.cpp \
          "

S ="${WORKDIR}"

inherit cmake

RPROVIDES_${PN} += "user-application"

