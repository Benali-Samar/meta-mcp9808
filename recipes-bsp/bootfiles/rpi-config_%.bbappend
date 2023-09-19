
SUMMARY = "Append file to edit the rpi configuration file for enabling the MCP9808 Driver"
DESCRIPTION = "${SUMMARY}"

# Editing the config.txt rpi file
do_deploy:append(){
	if [ ${ENABLE_MCP9808_DRIVER} -eq 1 ]; then
		echo "# MCP8908 Overlay" >> ${DEPLOYDIR}/bcm2835-bootfiles/config.txt
		echo "dtoverlay=mcp" >> ${DEPLOYDIR}/bcm2835-bootfiles/config.txt
	fi
}		
