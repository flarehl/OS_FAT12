/*
Author: Hanah Leo & Stephen Brikiatis		
Class:  CSI-385-01
Assignment: FAT12 Implementation
Date Assigned: Start of semester
Due Date: 11/02/2016

Description: Basic FAT12 implementation and shell handling some custom commands
Certification of Authenticity:
I certify that this assignment is entirely my own work.
*/



#include <stdio.h>
#include "./utilities/shmem.h"
#include "shell.h"

int main(int argc, char **argv)
{
	/* Initialize shared memory segment*/
	void *shPtr;
	char dir[] = "/";

	createShmem(&shPtr); //passing address of the pointer, int value

	memset(CPATH.path, '\0', MAX_PATH);

	strncpy(CPATH.path, dir, 1);
	CPATH.sectorNum = 0;
	memcpy(shPtr, &CPATH, SHMEMSIZE); //sets shared memory variables


	/* Shell */
	char** args;
	char* line;
	int status = 0;

	do {

		line = getInput(CPATH);
		args = parseInput(line);

		if(args == NULL)
			continue;

		if( (strncmp(args[0], "exit", 4) == 0) || (strncmp(args[0], "logout", 6) == 0) )
		{
			status = 1;
			continue;
		}

		executeCmd(args);

		memcpy(&CPATH, shPtr, SHMEMSIZE); //read in shared memory for updates, need in main?

	} while(status == 0);


	detachShmem(shPtr);

	return 0;
}

