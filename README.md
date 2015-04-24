# fch_xhci_rom_dumper

This program is for dumping the contents of the boot (mask) ROM of the uPD720200/200A XHCI controller used in the AMD FCHs.
The Tool is tested with the Bolton D4 (aka A88X) FCH, but should also work with the Hudson FCHs.
It might work with the real uPD720200/200A if you add the corresponding PCI IDs, but thats also untested.

You need the libpci headers installed to compile.

The Program needs to be run as root and will output the boot ROM binary of the found XHCI controllers in the system to a file with the location on the PCI bus and the vendor and device ID in the filename.

Run the Program at your own risk.

It's a quick-and-dirty written tool, which worked on my test platform.
If you're unsure what this tool is for, don't run it ;)
