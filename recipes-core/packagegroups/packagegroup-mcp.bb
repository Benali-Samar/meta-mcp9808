DESCRIPTION = " mcp9808 packagegroup"
SUMMARY = " MCP9808 packagegroup"


inherit packagegroup

RDEPENDS_${PN} = " mcp-driver \ 
		mcp-overlay kernel-modules"
