
SUMMARY = " MCP9808 packagegroup recipe "
DESCRIPTION = "${SUMMARY}"

# Heriting the packagegroupe.bbclass 
inherit packagegroup

# Run time dependencies 
RDEPENDS_${PN} = " mcp-driver mcp-overlay kernel-modules mcpapp "
