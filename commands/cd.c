#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "cmdSupport.h"

//incorporate cluster handling for large entries, maybe incorporate 

int main(int argc, char** argv) {

	void *shPtr;
	char rDir[] = "ROOT";
	char slash[] = "/";

	accessShmem(&shPtr); //passing address of the pointer, int value
	memset(CPATH.path, '\0', MAX_PATH);
	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory

	FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
	if (FILE_SYSTEM_ID == NULL) {
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	//if cwd is not root, translate logical sector number
	int numSector = CPATH.sectorNum;
	if(strcmp(CPATH.path,"ROOT") != 0)
		numSector += 31;

	if(argc == 1){ 
		memset(CPATH.path, '\0', sizeof(CPATH.path));
		strncpy(CPATH.path, rDir, 4);		
		CPATH.sectorNum = 19;
		memcpy(shPtr, &CPATH, SHMEMSIZE); //should send to cd
		return 0;

	}
	else if(argc == 2){

		FileData entry;
		CurrentPath tmp;

		entry = searchEntries(argv[1], numSector);

		if(strcmp(entry.fileName, "        ") != 0){

			if( !isFile(entry) ){

				strcat(CPATH.path, slash);
				strcat(CPATH.path, argv[1]);
				CPATH.sectorNum = entry.flc;

				memcpy(shPtr, &CPATH, SHMEMSIZE); //updates shmem for fat12.c
			}
			else
				printf("argument provided was a file, not a directory\n");

		} else
			printf("directory does not exist\n");
			
		

	} else {
		printf("cd may only handle 1 or 2 arguments\n");
	}


	detachShmem(shPtr);

	return 0;
}

