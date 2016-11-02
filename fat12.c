#include <stdio.h>
#include "./utilities/shmem.h"
#include "shell.h"

int main(int argc, char **argv)
{
	/* Initialize shared memory segment*/
	void *shPtr;
	CurrentPath cPath;
	char dir[] = "root/";

	createShmem(&shPtr); //passing address of the pointer, int value

	memset(cPath.path, '\0', MAX_PATH);

	strncpy(cPath.path, dir, 5);
	cPath.sectorNum = 19;
	memcpy(shPtr, &cPath, SHMEMSIZE); //should send to cd


	/* Shell */
	char** args;
	char* line;

	while( TRUE ) {

		line = getInput();
		args = parseInput(line);

		if( (strncmp(args[0], "exit", 4) == 0) || (strncmp(args[0], "logout", 6) == 0) )
			break; // use continue to break out of do while loop

		executeCmd(args);

	} 


	detachShmem(shPtr);

	return 0;
}

