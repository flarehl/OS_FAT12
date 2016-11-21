#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "cmdSupport.h"

//incorporate cluster handling for large entries, maybe incorporate 

int main(int argc, char** argv) {


	void *shPtr;
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
	if(argc == 1)
	{ 
		memset(CPATH.path, '\0', sizeof(CPATH.path));
		strncpy(CPATH.path, slash, 1);		
		CPATH.sectorNum = 0;
		memcpy(shPtr, &CPATH, SHMEMSIZE); 
		printf("%s\n", CPATH.path);
		return 0;

	}
	else if(argc == 2)
	{ 

		FileData *entry;
		CurrentPath tmp;
		char* tmpPath = (char*)malloc(SHMEMSIZE * sizeof(char)); 
		char** parsed;


		parsed = parsePath(argv[1]);

		if( (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0) && strcmp(CPATH.path, "/") == 0) 
			printf("user is already in root\n"); //refactor for more efficient handling

		else if(getArgc(parsed) > 1) //deal with path arguments
		{
			//reset CPATH
			memset(CPATH.path, '\0', MAX_PATH);
			strcat(CPATH.path, slash); //first slash for building CPATH.path

			int j = 0;
			for(j = 0; j < getArgc(parsed); j++)
			{
				if(strncmp(parsed[j], ".", 1) != 0 && strncmp(parsed[j], "..", 2) != 0)
					parsed[j] = fileTranslate(parsed[j]);
			}

			int i = 0;
			while(  i < getArgc(parsed) && ( (entry = searchEntries(parsed[i], numSector)) != NULL ))
			{
				if(isFile(entry)){
					printf("entry is not a directory\n");
					break;
				}

				CPATH.sectorNum = entry->flc; //set to previous sector
				if(entry->flc == 0)
					numSector = 19;
				else 
					numSector = entry->flc + 31;

				if(strncmp(parsed[i], ".", 1) != 0 && strncmp(parsed[i], "..", 2) != 0){
					strcat(CPATH.path, parsed[i]);
					strcat(CPATH.path, slash);
				} //fix bug where if using more than one . or .., the path is set incorrectly

				memcpy(shPtr, &CPATH, SHMEMSIZE); //updates shmem for fat12.c, can be put outside loop??
				i++;

			}


			if(entry == NULL)
				printf("directory does not exist\n");

		}	
		else if(getArgc(parsed) == 1) //handle pruning
		{
			char *name = argv[1];

			if(strncmp(name, ".", 1) != 0 && strncmp(name, "..", 2) != 0){
				name = fileTranslate(argv[1]);
			}	

			if((entry = searchEntries(name, numSector)) != NULL)
			{

				if( !isFile(entry) )
				{
					if(strncmp(name, ".", 1) != 0 && strncmp(name, "..", 2) != 0)
					{
						strcat(CPATH.path, name);
						strcat(CPATH.path, slash);
					}
					else if(strncmp(name, "..", 2) == 0)
					{
						char tmp[1024]; //find out why later
						char** parsedTmp;

						strcpy(tmp, CPATH.path);
						parsedTmp = parsePath(tmp);

						//rebuild CPATH
						memset(CPATH.path, '\0', MAX_PATH);
						strcat(CPATH.path, slash); 

						int i = 0;
						while(i < getArgc(parsedTmp) - 1){
							strcat(CPATH.path, parsedTmp[i]);
							strcat(CPATH.path, slash); 
							i++;
						}
					}

					CPATH.sectorNum = entry->flc;
					memcpy(shPtr, &CPATH, SHMEMSIZE);

				}
				else
					printf("argument provided was a file, not a directory\n");


			} else
				printf("directory does not exist\n");

		}



	} else 
		printf("cd may only handle 1 or 2 arguments\n");
	

	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory
	printf("%s\n", CPATH.path);

	detachShmem(shPtr);

	return 0;
}

