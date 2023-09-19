
SUMMARY = "Image recipe for for MCP9808 I2C sensor's driver integration with its userspace applications"

# Requiring the core-image-minimal.bb as a base of our image
require recipes-core/images/core-image-minimal.bb


# Adding packages to the image
IMAGE_INSTALL:append = " mcp-overlay kernel-modules"
IMAGE_INSTALL:append = " mcp-driver"
IMAGE_INSTALL:append = " packagegroup-mcp"
IMAGE_INSTALL:append = " mcpapp"

# Adding the device tree overlay to the image
RPI_KERNEL_DEVICETREE_OVERLAYS:append = " overlays/mcp.dtbo"
