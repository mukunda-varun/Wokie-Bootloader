# Wokie-Bootloader
F407 Bootloader code with Version check
STM32F407 bootloader code with firmware version check at address 0x8060000(ADDRESS_TO_READ)

Memory Partition for the Wokie project is done as below:


Section       |   Start Address   |     Size
Bootloader    |    0x8000000      |     128KB
Application   |    0x8040000      |     128KB
FW Version    |    0x8060000      |     4KB

Partitions are done in respective code in flash.ld file
The VECT_TAB_OFFSET value in application code has to be changed to VECT_TAB_OFFSET  0x00040000 from 0x00
