#include <stdio.h>
#include "cmdSupport.h"

bool isLongFile(FileData);

int main(int argc, char** argv) {

	void *shPtr;

	accessShmem(&shPtr); //passing address of the pointer, int value
	memset(CPATH.path, '\0', MAX_PATH);
	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory


	FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
	if (FILE_SYSTEM_ID == NULL) {
	  printf("Could not open the floppy drive or image.\n");
	  exit(1);
	}
	char *buffer = (char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	FileData nEntry; //for emptying entry
	FileData entry;
	int offset = 0,
		numSector = CPATH.sectorNum;

	//if cwd is not root, translate logical sector number
	if(strcmp(CPATH.path,"ROOT") != 0)
		numSector += 31;


	printf("%-13s %7s %12s %6s\n", "File Name", "Type", "File Size", "FLC");
	printf("---------------------------------------------\n");

	do { //for reading in entries from struct

		if (read_sector(numSector, buffer) == -1) {
			printf("Something has gone wrong -- could not read the boot sector\n");
			return -1;
		}

		for(int i = 0; i < 16; i++){ // 16 entries per sector

			entry = nEntry; //to reset values, spaghettiiiiiii

			entry = readEntry(buffer, &offset);

			if( entry.fileName[0] == (char)0x00 || entry.fileName[0] == (char)0xf6){ //needs an or with 0xF6
				break; //fix this later
			}
			else if( entry.fileAttributes == (char)0x0f || entry.fileName[0] == (char)0xE5 ){
				continue;

			}else{

				/* HANDLE THE RELATIVE AND ABSOLUTE */

				//space delimit to get rid of padding for file name and concatenate with dot before extension 
				char* name = strtok(entry.fileName, " ");
				if(entry.fileExt[0] != ' '){
				 	char dot[] = "."; //strcat doesnt play well with 
					strcat(name, dot);
				}

				//wrap into char* getType() later
				char file[] = "file";
				char dir[] = "dir";

				char* type;
				if(isFile(entry))
					type = file;
				else
					type = dir;
				

				printf("%-13s %7s %12d %6d\n", strcat(name, entry.fileExt), type, entry.fileSize, entry.flc);
			}

		}


		// if entire sector is read, then keep reading next sector in root directory
		if(entry.fileName[0] != (char)0x00 && entry.fileName[0] != (char)0xE5 ){
		 	numSector++;
		}
		
		offset = 0;

	} while (entry.fileName[0] != (char)0x00 && numSector < 32);

	return 0;
}





