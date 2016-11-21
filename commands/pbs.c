#include <stdlib.h>
#include <stdio.h>
#include "cmdSupport.h"

/* Prototypes */
void printBootSector();
int readBootSector();


/* Main */
int main(int argc, char** argv) {

	if(argc != 1)
	{
		printf("Too many arguments\n");
		exit(1);
	}

	readBootSector();
	printBootSector();

}


/* Functions */

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


/*****************************************************************************
* readBootSector
*
* reads and assigns values for BootSector object which can get displayed
*
* Return: 0, or -1 if the read fails.
****************************************************************************/
int readBootSector() {

	unsigned char* buffer;
	int mostSignificantBits, leastSignificantBits;

	FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");

	if (FILE_SYSTEM_ID == NULL)
	{
		printf("Here? Could not open the floppy drive or image.\n");
		exit(1);
	}

	/*
		change temporarily, otherwise wasting time reading unneeded data
		read_sector uses BYTES_PER_SECTOR so cant just directly use BYTES_TO_READ_IN_BOOT_SECTOR
		afterwards, reset BYTES_PER_SECTOR by using bytesPerSector variable 
	*/
	BYTES_PER_SECTOR = BYTES_TO_READ_IN_BOOT_SECTOR;

	buffer = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	if (read_sector(0, buffer) == -1) {
		printf("Something has gone wrong -- could not read the boot sector\n");
		return -1;
	}

	/*  
		Larger index first because little endian -> see board
		why is it << 8? BECAUSE OR IS A UNION!!!!!!!!!! if 4 bytes, shift 24
		lots of redundancy, but easier to understand this way
	*/

	// sets bytes per sector	*/
	mostSignificantBits = (((int)buffer[12]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[11]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.bytesPerSector = mostSignificantBits | leastSignificantBits;

	// sets sector per cluster 
	BOOT_SECTOR_ATTRIBUTES.sectorsPerCluster = (int)buffer[13];

	// sets number of reserved sectors
	mostSignificantBits = ((int)buffer[15] << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[14]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.numReserved = mostSignificantBits | leastSignificantBits;

	// sets number of FAT tables
	BOOT_SECTOR_ATTRIBUTES.numFATs = (int)buffer[16];

	// sets max number of root directories
	mostSignificantBits = (((int)buffer[18]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[17]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.numRootEntries = mostSignificantBits | leastSignificantBits;

	// sets total sector count
	mostSignificantBits = (((int)buffer[20]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[19]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.totalSectorCount = mostSignificantBits | leastSignificantBits;

	// sets sectors per FAT
	mostSignificantBits = (((int)buffer[23]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[22]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.sectorPerFAT = mostSignificantBits | leastSignificantBits;

	// sets sectors per track
	mostSignificantBits = (((int)buffer[25]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[24]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.sectorPerTrack = mostSignificantBits | leastSignificantBits;

	// sets number of heads
	mostSignificantBits = (((int)buffer[27]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[26]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.numHeads = mostSignificantBits | leastSignificantBits;

	// sets boot signature
	BOOT_SECTOR_ATTRIBUTES.bootSig = buffer[38];

	// sets volume ID, 4 bytes so << 24 
	mostSignificantBits = (((int)buffer[42]) << 24) & 0xff000000;
	int midBits = ((int)buffer[41] << 16) & 0x00ff0000;
	int midBits2 = ((int)buffer[40] << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[39]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.volumeID = (mostSignificantBits | midBits | midBits2 | leastSignificantBits) ;


	/***** for loops make this way easier for reading char values *****/

	// sets volume label, 11 bytes + null
	int j = 43;
	int i = 0;
	for (i = 0; i < 11; i++) {
		BOOT_SECTOR_ATTRIBUTES.volumeLabel[i] = buffer[j];
		j++;
	}
	
	// sets file system type, 8 bytes 54 to 61 + null
	j = 54;
	for (i = 0; i < 8; i++) {
		BOOT_SECTOR_ATTRIBUTES.systemType[i] = buffer[j];
		j++;
	}

	//reset BYTES_PER_SECTOR to 512 for use everywhere else
	BYTES_PER_SECTOR = BOOT_SECTOR_ATTRIBUTES.bytesPerSector;

	return 0;
}
