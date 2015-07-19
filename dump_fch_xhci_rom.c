#include <stdint.h>
#include <stdio.h>
#include <pci/pci.h>

#define INDIRECT_PCI_INDEX_REG	0x48
#define INDIRECT_PCI_DATA_REG	0x4C

#define IND_60MHZ_SPACE_ROMTESTCS_REG	0x40
#define IND_60MHZ_SPACE_ROMTESTDATA_REG	0x44

#define ROMTESTCS_TEST_START	(1 << 0)
#define ROMTESTCS_TESTMODE_READ	(1 << 1)
#define ROMTESTCS_DONE	(1 << 30)

#define ROM_SIZE_DWORDS	0x2000


void write_60mhz_space_register(struct pci_dev *dev, uint32_t addr, uint32_t value){
	pci_write_long(dev, INDIRECT_PCI_INDEX_REG, (addr & 0x3FFFFFFF) | 0x40000000);
	pci_write_long(dev, INDIRECT_PCI_DATA_REG, value);
}

uint32_t read_60mhz_space_register(struct pci_dev *dev, uint32_t addr){
	pci_write_long(dev, INDIRECT_PCI_INDEX_REG, (addr & 0x3FFFFFFF) | 0x40000000);
	return pci_read_long(dev, INDIRECT_PCI_DATA_REG);
}


uint32_t read_rom_dword(struct pci_dev *dev, uint32_t dword_addr){
	uint32_t cmd = ROMTESTCS_TEST_START | ROMTESTCS_TESTMODE_READ | ((dword_addr & (ROM_SIZE_DWORDS-1)) << 2);
	write_60mhz_space_register(dev, IND_60MHZ_SPACE_ROMTESTCS_REG, cmd);
	while(!(read_60mhz_space_register(dev, IND_60MHZ_SPACE_ROMTESTCS_REG) & ROMTESTCS_DONE)){;}
	return read_60mhz_space_register(dev, IND_60MHZ_SPACE_ROMTESTDATA_REG);
}


void dump_rom_to_file(struct pci_dev *dev){
	char filename[64];
	sprintf(filename, "%X.%X.%X_%X_%X.bin", dev->bus, dev->dev, dev->func, dev->vendor_id, dev->device_id);
	FILE *fp = fopen(filename, "wb");
	int i;
	
	for(i=0; i<ROM_SIZE_DWORDS; i++){
		uint32_t temp = read_rom_dword(dev, i);
		fwrite(&temp, sizeof(temp), 1, fp);
	}
	
	fclose(fp);
}


int main(void)
{
	struct pci_access *pacc;
	struct pci_dev *dev;

	pacc = pci_alloc();
	pci_init(pacc);
	pci_scan_bus(pacc);
	for (dev=pacc->devices; dev; dev=dev->next){
		pci_fill_info(dev, PCI_FILL_IDENT);
		if(0x1022 == dev->vendor_id && (0x7814 == dev->device_id || 0x7812 == dev->device_id)){
			dump_rom_to_file(dev);
		}
	}
	pci_cleanup(pacc);
	return 0;
}
