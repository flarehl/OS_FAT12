#include <stdio.h>
#include "cmdSupport.h"

bool isLongFile(FileData);

int main(int argc, char** argv) {

	FILE_SYSTEM_ID = fopen("./floppies/floppy1", "r+");
	if (FILE_SYSTEM_ID == NULL) {
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	char *buffer = (char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
	FileData entry;

	int rootSector = 19,
		offset = 0,
		numSectors = 0;

	printf("%-12s %7s %12s %6s\n", "File Name", "Type", "File Size", "FLC");

	do { //for reading in entries from struct

		if (read_sector(rootSector, buffer) == -1) {
			printf("Something has gone wrong -- could not read the boot sector\n");
			return -1;
		}

		for(int i = 0; i < 16; i++){

			FileData nEntry;
			entry = nEntry; //to reset values, spaghetti

			entry = readEntry(buffer, &offset);

			if( entry.fileAttributes == 0x0f ){
				/* TEST */ printf("ignore this 32 byte entry\n");
				continue;
			}else{
				//display formatting
				printf("%-12s %7s %12d %6d\n", strcat(entry.fileName, entry.fileExt), "type", entry.fileSize, entry.flc);
			}

		}

		// if entire sector is read, then keep reading next sector in root directory
		if(entry.fileName != "")
		 	rootSector++;
		
	} while (entry.fileName[0] != 0x00 && numSectors < 14);

	return 0;
}


/******************************************************************************
* isLongFile
*
* Checks if file entry needs to be ignored
*
* entry:  file entry with name to be checked
*
* Return: true if file entry should be ignored
*****************************************************************************/
bool isLongFile(FileData entry){

	if(entry.fileAttributes == 0x0f)
		return TRUE;
	else 
		return FALSE;

}


