# fch_xhci_rom_dumper

This program is for dumping the contents of the boot (mask) ROM of the uPD720200/200A XHCI controller used in the AMD FCHs.
The tool is tested with Bolton D4 (aka A88X) and Hudson D4 (aka A85X) FCH.

You need the libpci headers installed to compile.

The Program needs to be run as root and will output the boot ROM binary of the found XHCI controllers in the system to a file with the location on the PCI bus and the vendor and device ID in the filename.

Run the Program at your own risk.

It's a quick-and-dirty written tool, which worked on my test platform.
If you're unsure what this tool is for, don't run it ;)

## address space layout seen from the V850E core

0x00000000-0x00007FFF: boot ROM (extracted by this tool)

0x00008000-0x0000FFFF: firmware RAM (code partition without version bytes of the firmware binary)

