#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "fatSupport.h"

//prototypes
bool checkRange(int x, int y);
unsigned char* readFAT12Table(int FAT_Number);

//pfe program
int main(int argc, char** argv) {

	unsigned char* fatBuffer;

	if(argc > 3 || argc < 2){
		printf("Error: pfe only accepts 2 arguments\n");
		exit(1);
	}

	//validate if arguments are char, cast to int should handle it??

	// argument validation - in fatSupport
	if ( checkRange( atoi(argv[1]), atoi(argv[2]) ) ) {

		fatBuffer = readFAT12Table(1);

		//display results
		for(int i = atoi(argv[1]); i <= atoi(argv[2]); i++)
			printf( "Entry %d: %x\n", i, get_fat_entry(i, fatBuffer) );
		
		return 0;

	}
	else {
		printf("%s\n", "Error: Invalid argument(s)\n");
		getchar();
		exit(1);
	}

}


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

	if (x < 2) {
		printf("%s\n", "Error: first sector cannot be below the value 2");
		return FALSE;
	}
	else if (x > y) {
		printf("%s\n", "Error: first sector value cannot be over the second sector value ");
		return FALSE;
	}
	else
		return TRUE;

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