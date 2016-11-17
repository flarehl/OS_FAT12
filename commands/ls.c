#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char** argv) {

	if(argc < 1 || argc > 2){
		printf("command supports 0-1 arguments\n");
		return -1;
	}

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

	FileData* nEntry; //for emptying entry
	FileData* entry;
	int offset = 0,
		numSector = CPATH.sectorNum;

	//translate to physical sec num
	if(CPATH.sectorNum == 0)
		numSector = 19;
	else
		numSector += 31;


	printf("_____________________________________________\n");
	printf("%-13s %7s %12s %6s\n", "File Name", "Type", "File Size", "FLC");
	printf("_____________________________________________\n");

	do { //for reading in entries from struct

		/* 
			if argc is 2, then parse the path argument and search for each entry
			if the last filename is a file(last token before NULL) then show entry attributes
			if last filename is a directory just do the thing underneath
		*/


		if (read_sector(numSector, buffer) == -1) {
			printf("Something has gone wrong -- could not read the sector\n");
			return -1;
		}


		if(argc == 2){

			int i = 0;
			char** tmp;
			char* last;
			tmp = parsePath(argv[1]); 

			// gets the last filename that needs to be displayed
			while(tmp[i] != NULL){
				last = tmp[i];

				if((numSector = getSectorOffset(tmp[i], buffer)) == -1)
					return -1;

				i++;
			}

			//if items exists until (last, tmp[i]), then searchEntries
			//* TEST */printf("tmp[%i]: %s\n", i, tmp[i]);



		}

		for(int i = 0; i < 16; i++){ // 16 entries per sector

			entry = nEntry; //to reset values, spaghettiiiiiii

			if( (entry = readEntry(buffer, &offset)) == NULL )
				break;

			if(strcmp(CPATH.path,"ROOT") != 0 && strcmp(CPATH.path,"ROOT/") != 0)
				CPATH.sectorNum += 31;

			if( isEmpty(entry) || entry->fileName[0] == (char)0xf6){ //needs an or with 0xF6
				break; //fix this later
			}
			else if( isLongFile(entry) || isDeleted(entry) ){
				continue;

			}else{

				//space delimit to get rid of padding for file name and concatenate with dot before extension 
				char* name = strtok(entry->fileName, " ");
				if(entry->fileExt[0] != ' '){
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
				

				printf("%-13s %7s %12d %6d\n", strcat(name, entry->fileExt), type, entry->fileSize, entry->flc);
			}

		}


		// if entire sector is read, then keep reading next sector in root directory
		if(!isEmpty(entry) && !isDeleted(entry) ){
		 	numSector++;
		}
		
		offset = 0;

	} while (!isEmpty(entry) && numSector < 32);

	return 0;
}





