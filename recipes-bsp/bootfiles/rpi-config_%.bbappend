do_deploy_append(){
	if [ ${ENABLE_MCP9808_DRIVER} -eq 1 ]; then
		echo "# MCP8908 Overlay" >> ${DEPLOYDIR}/bcm2835-bootfiles/config.txt
		echo "dtoverlay=mcp" >> ${DEPLOYDIR}/bcm2835-bootfiles/config.txt
	fi
}		
