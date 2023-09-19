SUMMARY = "mcp9808 Overlay Recipe"
LICENSE = "CLOSED"
SECTION = "bsp"
inherit devicetree deploy

SRC_URI = "file://mcp.dts"

do_deploy(){
    install -d ${DEPLOYDIR}
    install -m 0644 ${B}/mcp.dtbo ${DEPLOYDIR}
}

COMPATIBLE_MACHINE = "raspberrypi"
FILES_${PN} = "/boot/devicetree/mcp.dtbo"
