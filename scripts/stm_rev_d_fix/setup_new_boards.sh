# Script to apply openocd config files for Makefile 
# Revision Oct. 3, 2017
# Fixes problem of stm32f303 discovery boards of revision D

echo "Running script to fix stlink"
cp stm32f3discovery_revD.cfg ~/ee155_tools/openocd_src/tcl/board/stm32f3discovery_revD.cfg 
cp stm32f3discovery_revD.cfg ~/ee155_tools/openocd/share/openocd/scripts/board/stm32f3discovery_revD.cfg
