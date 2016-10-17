#ifndef BOOT_ATTRIBUTES_H
#define BOOT_ATTRIBUTES_H

#define BYTES_TO_READ_IN_BOOT_SECTOR 62

typedef struct {

	unsigned int bytesPerSector;
	unsigned int sectorsPerCluster;
	unsigned int numFATs;
	unsigned int numReserved;
	unsigned int numRootEntries;
	unsigned int totalSectorCount;
	unsigned int sectorPerFAT;
	unsigned int sectorPerTrack;
	unsigned int numHeads;
	char* bootSig;
	char* volumeID;

	//add one to byte value bc NULL character for char arrays
	char volumeLabel[12]; 
	char systemType[9];

} BootSector;

#endif
