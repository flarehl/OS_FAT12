#include <stdlib.h>
#include <stdio.h>
#include "fatSupport.h"

void printBootSector();

//change later to handle command arguments for shell
int main(int argc, char** argv) {

	if(argc > 1){
		printf("Too many arguments\n");
		exit(1);
	}

	readBootSector();
	printBootSector();

}

/******************************************************************************
* printBootsector
*
* displays boot sector info to user
*
*****************************************************************************/
void printBootSector() {

	printf("Bytes per sector: %14d\n", BOOT_SECTOR_ATTRIBUTES.bytesPerSector);
	printf("Sectors per cluster: %9d\n", BOOT_SECTOR_ATTRIBUTES.sectorsPerCluster);
	printf("Number of FATs: %14d\n", BOOT_SECTOR_ATTRIBUTES.numFATs);
	printf("Number of reserved sectors: %2d\n", BOOT_SECTOR_ATTRIBUTES.numReserved);
	printf("Number of root entries: %8d\n", BOOT_SECTOR_ATTRIBUTES.numRootEntries);
	printf("Total sector count: %13d\n", BOOT_SECTOR_ATTRIBUTES.totalSectorCount);
	printf("Sectors per FAT: %13d\n", BOOT_SECTOR_ATTRIBUTES.sectorPerFAT);
	printf("Sectors per track: %12d\n", BOOT_SECTOR_ATTRIBUTES.sectorPerTrack);
	printf("Number of heads: %15d\n", BOOT_SECTOR_ATTRIBUTES.numHeads);

	printf("Boot Signature (in hex):     %#03x\n", BOOT_SECTOR_ATTRIBUTES.bootSig);
	printf("Volume ID:                   %#03x\n", BOOT_SECTOR_ATTRIBUTES.volumeID);
	printf("Volume Label: %26s\n", BOOT_SECTOR_ATTRIBUTES.volumeLabel);
	printf("File System Type: %19s\n", BOOT_SECTOR_ATTRIBUTES.systemType);

}

