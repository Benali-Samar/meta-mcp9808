

include recipes-core/images/core-image-minimal.bb


IMAGE_INSTALL:append = " mcp-overlay kernel-modules"
IMAGE_INSTALL:append = " mcp-driver"
IMAGE_INSTALL:append = " packagegroup-mcp"
IMAGE_INSTALL:append = " mcpapp"

RPI_KERNEL_DEVICETREE_OVERLAYS:append = " overlays/mcp.dtbo"
