include recipes-core/images/core-image-minimal.bb


IMAGE_INSTALL_append = " mcp-overlay kernel-modules"
IMAGE_INSTALL_append = " mcp-driver"
IMAGE_INSTALL_append = " packagegroup-mcp"

RPI_KERNEL_DEVICETREE_OVERLAYS_append = " overlays/mcp.dtbo"
