#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "cmdSupport.h"

int main(int argc, char** argv) {

	void *shPtr;
	char rDir[] = "root/";
	char **parsedPath;

	accessShmem(&shPtr); //passing address of the pointer, int value

	memset(CPATH.path, '\0', MAX_PATH);
	//use strcpy to tack on next part of path
	//if needs to go back, traverse back to dir before current

	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory

	FILE_SYSTEM_ID = fopen("./floppies/floppy1", "r+");
	if (FILE_SYSTEM_ID == NULL) {
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	if(argc == 1){ 
		strncpy(CPATH.path, rDir, 5);		
		CPATH.sectorNum = 19;
		return 0;

	}
	else if(argc == 2){

		parsedPath = parsePath(argv[1]);

		/* TEST */ printf("%s\n", parsedPath[0]);


		if( isAbsolutePath(parsedPath[0]) ){
			//search with isFileExist()
		}
		else if( isRelativePath(parsedPath[0]) ) {
			//if sizeof(parsedPath) == 1, check if '.' or '..'
			//read wiki about relative filename[0] == 0x2E
		}

	} else {
		printf("cd may only handle 1 or 2 arguments\n");
	}


	detachShmem(shPtr);

	return 0;
}



/* 

Functions that might be needed:

	void moveUpDirectory()
	void moveDownDirectory()
	bool isFile() <- in fatSupport because used in different programs

*/