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

	//if cwd is not root, translate logical sector numbe
	int numSector = CPATH.sectorNum;
	if(numSector == 0)
		numSector = 19;
	else 
		numSector += 31;

	//if argument is blank
	if(argc == 1){ 
		memset(CPATH.path, '\0', sizeof(CPATH.path));
		strncpy(CPATH.path, rDir, 4);		
		CPATH.sectorNum = 19;
		memcpy(shPtr, &CPATH, SHMEMSIZE); 
		return 0;

	}
	else if(argc == 2){ 

		FileData *entry;
		CurrentPath tmp;
		char* tmpPath = (char*)malloc(SHMEMSIZE * sizeof(char)); 
		char* last;
		char** parsed;

		if( (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0) && strcmp(CPATH.path, "ROOT") == 0) 
			printf("user is already in root\n");

		else if(strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0){

			if((entry = searchEntries(argv[1], numSector)) != NULL && strcmp(argv[1], "..") == 0){

				strcpy(tmpPath, CPATH.path); //copy path for memory
				parsed = parsePath(tmpPath);

				memset(CPATH.path, '\0', MAX_PATH);

				int i = 0;
				// gets the last filename that needs to be pruned
				while(parsed[i] != NULL){
					last = parsed[i];
					i++;
				}

				CPATH.sectorNum = entry->flc; //set to previous sector

				i = 0;
				// recreate CPATH
				while(parsed != NULL){

					if(strcmp(last, parsed[i]) != 0){
						strcat(CPATH.path, parsed[i]);
						strcat(CPATH.path, slash);
					}
					i++;
					memcpy(shPtr, &CPATH, SHMEMSIZE); //updates shmem for fat12.c

				}

			} else if(strcmp(argv[1], ".") == 0 ){
				//do nothing, print the CPATH.path
			}
			else
				printf(". and .. unavailable\n");

		}	
		else {

			if((entry = searchEntries(argv[1], numSector)) != NULL){

				if( !isFile(*entry) ){

					strcat(CPATH.path, argv[1]);
					strcat(CPATH.path, slash);
					CPATH.sectorNum = entry->flc;

					memcpy(shPtr, &CPATH, SHMEMSIZE); //updates shmem for fat12.c
				}
				else
					printf("argument provided was a file, not a directory\n");


			} else
				printf("directory does not exist\n");

		}

	} else 
		printf("cd may only handle 1 or 2 arguments\n");
	


	detachShmem(shPtr);

	return 0;
}

