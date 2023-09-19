
SUMMARY = "A simple user space application for plotting driver's temperature"
DESCRIPTION = "${SUMMARY}"

# No license specified
LICENSE = "CLOSED"

# Sources
SRC_URI = "git://github.com/Benali-Samar/MCP_apps.git;protocol=https;branch=main"
SRCREV = "6c3e0dc8cdfb34839684c7b938de8c6d06d83555"

# Unpacking
S = "${WORKDIR}/git"

# Dependencies
DEPENDS += "python3"

# Run time dependencies
RDEPENDS_${PN} += "python3-module-os"

# Installing
do_install() {
    install -d ${D}/usr/bin
    install -m 0755 App_plot/plot.py ${D}/usr/bin
}

# Packaging
FILES_${PN} += "${libdir}/python3.7/site-packages/*"



