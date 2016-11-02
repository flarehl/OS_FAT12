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

	int offset = 0,
		numSector = 19;

	printf("%-13s %7s %12s %6s\n", "File Name", "Type", "File Size", "FLC");

	do { //for reading in entries from struct

		if (read_sector(numSector, buffer) == -1) {
			printf("Something has gone wrong -- could not read the boot sector\n");
			return -1;
		}

		for(int i = 0; i < 16; i++){ // 16 entries per sector

			FileData nEntry;
			entry = nEntry; //to reset values, spaghettiiiiiii

			entry = readEntry(buffer, &offset);

			if( entry.fileName[0] == (char)0x00 ){
				break; //fix this later
			}
			else if( entry.fileAttributes == (char)0x0f || entry.fileName[0] == (char)0xE5 ){
				continue;

			}else{
				//space delimit to get rid of padding for file name and concatenate with dot before extension 
				char* name = strtok(entry.fileName, " ");
				if(entry.fileExt[0] != ' '){
					char dot[] = ".";
					strcat(name, dot);
				}

				printf("%-13s %7s %12d %6d\n", strcat(name, entry.fileExt), "type", entry.fileSize, entry.flc);
			}

		}


		// if entire sector is read, then keep reading next sector in root directory
		if(entry.fileName[0] != (char)0x00 && entry.fileName[0] != (char)0xE5 ){
		 	numSector++;
		}
		else
			continue;
		
	} while (entry.fileName[0] != (char)0x00 && entry.fileName[0] != (char)0xE5 && numSector < 32);

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


