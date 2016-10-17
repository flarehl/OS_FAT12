/******************************************************************************
 * Supporting functions for the FAT project:
 *
 *  read_sector
 *  write_sector
 *
 *  get_fat_entry
 *  set_fat_entry
 *  
 * Authors: Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *          March, 2004.
 *****************************************************************************/

#include <stdio.h>
#include "fatSupport.h"
#include "bootSector.h"
#include "FAT_Entry.h"

/******************************************************************************
 * FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                   filesystem)
 * BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 * BOOT_SECTOR_ATTRIBUTES -- a structure holding boot sector values
 *****************************************************************************/
extern FILE* FILE_SYSTEM_ID;
extern int BYTES_PER_SECTOR;
extern BootSector BOOT_SECTOR_ATTRIBUTES;

/******************************************************************************
* checkRange
*
* User command argument validation
*
* x:  first sector value to begin read
* y:  second sector value to end at 
*
* Return: true or false depending on range
*****************************************************************************/
bool checkRange(int x, int y) {

	if (x > 2) {
		printf("%s\n", "Error: first sector cannot be below the value 2");
		return FALSE;
	}
	else if (x > y) {
		printf("%s\n", "Error: first sector value cannot be over the value ");
		return FALSE;
	}
	else
		return TRUE;

}


/******************************************************************************
 * read_sector
 *
 * sector_number:  The number of the sector to read (0, 1, 2, ...)
 * buffer:  The array into which to store the contents of the sector that is
 *          read
 *
 * Return: the number of bytes read, or -1 if the read fails.
 *****************************************************************************/
int read_sector(int sector_number, unsigned char* buffer)
{
   int bytes_read;

   if (fseek(FILE_SYSTEM_ID, (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0)
   {
	   printf("Error accessing sector %d\n", sector_number);
      return -1;
   }

   bytes_read = fread(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

   if (bytes_read != BYTES_PER_SECTOR)
   {
      printf("Error reading sector %d\n", sector_number);
      return -1;
   }

   return bytes_read;
}


/*****************************************************************************
 * write_sector
 *
 * Write the contents of the given buffer to the filesystem at the specified
 * sector
 *
 * sector_number:  The number of the sector to write (0, 1, 2, ...)
 * buffer:  The array whose contents are to be written
 *
 * Return: the number of bytes written, or -1 if the read fails.
 ****************************************************************************/
int write_sector(int sector_number, unsigned char* buffer) 
{
   int bytes_written;

   if (fseek(FILE_SYSTEM_ID,
       (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0) 
   {
      printf("Error accessing sector %d\n", sector_number);
      return -1;
   }

   bytes_written = fwrite(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

   if (bytes_written != BYTES_PER_SECTOR) 
   {
      printf("Error reading sector %d\n", sector_number);
      return -1;
   }

   return bytes_written;
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

	FILE_SYSTEM_ID = fopen("floppy1", "r+");

	if (FILE_SYSTEM_ID == NULL)
	{
		printf("Could not open the floppy drive or image.\n");
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
	mostSignificantBits = (((int)buffer[12]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[11]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.numHeads = mostSignificantBits | leastSignificantBits;

	// sets boot signature
	BOOT_SECTOR_ATTRIBUTES.bootSig = (int)buffer[38];

	// sets volume ID, 4 bytes so << 24 
	mostSignificantBits = (((int)buffer[42]) << 24) & 0xff000000;
	int midBits = ((int)buffer[41] << 16) & 0x00ff0000;
	int midBits2 = ((int)buffer[40] << 8) & 0x0000ff00;
	leastSignificantBits = ((int)buffer[39]) & 0x000000ff;
	BOOT_SECTOR_ATTRIBUTES.volumeID = (mostSignificantBits | midBits | midBits2 | leastSignificantBits);


	/***** for loops make this way easier for reading char values *****/

	// sets volume label, 11 bytes
	int j = 43;
	int i = 0;
	for (i = 0; i < 11; i++) {
		BOOT_SECTOR_ATTRIBUTES.volumeLabel[i] = buffer[j];
		j++;
	}
	
	// sets file system type, 8 bytes 54 to 61 + null?/
	j = 54;
	for (i = 0; i < 8; i++) {
		BOOT_SECTOR_ATTRIBUTES.systemType[i] = buffer[j];
		j++;
	}

	//reset BYTES_PER_SECTOR to 512 for use everywhere else
	BYTES_PER_SECTOR = BOOT_SECTOR_ATTRIBUTES.bytesPerSector;

	return 0;
}


/*****************************************************************************
 * get_fat_entry
 *
 * Get the specified entry from the given FAT
 *
 * fat_entry_number:  The number of the FAT entry to get (0, 1, 2, ...)
 * fat:  The fat table from which to get the specified entry
 *
 * printf("Directory listing returned %d entries:\n", dnum);
 * Return: the value at the specified entry of the given FAT
 ****************************************************************************/
int get_fat_entry(int fat_entry_number, unsigned char* fat) 
{
   int offset;
   int uv, wx, yz;

   offset = 3 * fat_entry_number / 2;

   // Two FAT12 entries are stored into three bytes;
   // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw

   // odd fat entry number, return yzw
   if (fat_entry_number & 0x0001) 
   {
      wx = (int) fat[offset];
      yz = (int) fat[offset + 1];
      return ( (yz << 4)  |  ( (wx & 0x00f0) >> 4));
   } 
   // even fat entry number, return xuv
   else 
   {
      uv = (int) fat[offset];
      wx = (int) fat[offset + 1];
      return ( ((wx & 0x000f) << 8)  |  uv );
   }
}


/******************************************************************************
 * set_fat_entry
 *
 * Set the specified entry in the given FAT to the given value
 *
 * fat_entry_number:  The number of the FAT entry to set (0, 1, 2, ...)
 * value:  The given value to place in the FAT entry
 * fat:  The fat table in which to set the given value at the specified entry
 *****************************************************************************/
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat) 
{
   int offset;
   int uv, wx, yz, a, b, c;

   offset = 3 * fat_entry_number / 2;

   // Two FAT12 entries are stored into three bytes;
   // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw
   // Let 0a,bc denote the fat_entry_number, written as two bytes (high and
   // low, respectively)

   a = value & 0x0f00;
   b = value & 0x00f0;
   c = value & 0x000f;

   // odd fat entry number, change yzw to abc, i.e.,
   if (fat_entry_number & 0x0001) 
   {
      // wx = cx;
      fat[offset]     = (unsigned char) ((c << 4)  |  (fat[offset] & 0x000f));
      // yz = ab;
      fat[offset + 1] = (unsigned char) ((a >> 4)  |  (b >> 4));
   }
   // even fat entry number, change xuv to abc, i.e.,
   else
   {
      // uv = bc;
      fat[offset]     = (unsigned char) (b | c);
      // wx = wa;
      fat[offset + 1] = (unsigned char) ((fat[offset + 1]  & 
                                          0x00f0)  |  (a >> 8));
   }
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


/******************************************************************************
* readFAT12Table
*
* Reads the FAT12 tables 
*
* sectorNumber:  The number of the FAT entry
*
*****************************************************************************/
unsigned char* readFAT12Table(int FAT_Number) {

	unsigned char* fat = (unsigned char*)malloc( 9 * BYTES_PER_SECTOR * sizeof(unsigned char) );

	FILE_SYSTEM_ID = fopen("floppy1", "r+");

	if (FILE_SYSTEM_ID == NULL)
	{
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	// i is 0-numSectors, fat holds the data
	if (FAT_Number == 1) {
		for (int i = 0; i < 9; i++) 
			read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
		return fat;
	}
	else if (FAT_Number == 2) {
		for (int i = 10; i < 19; i++)
			read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
		return fat;
	}
	else
		printf("%s\n", "Error: invalid FAT table number");
	

}