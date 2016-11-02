#include <stdio.h>
#include "./utilities/shmem.h"
#include "shell.h"

int main(int argc, char **argv)
{
	/* Initialize shared memory segment*/
	void *shPtr;
	CurrentPath cPath;
	char dir[] = "root";

	createShmem(&shPtr); //passing address of the pointer, int value

	memset(cPath.path, '\0', MAX_PATH);

	for(int i = 0; i < 4; i++)
		cPath.path[i] = dir[i];
	cPath.sectorNum = 19;
	memcpy(shPtr, &cPath, SHMEMSIZE); //should send to cd


	/* Shell */
	char** args;
	char* line;

	while( TRUE ) {

		line = getInput();
		args = parseInput(line);

		if( (strncmp(args[0], "exit", 4) == 0) || (strncmp(args[0], "logout", 6) == 0) )
			break; // don't use break; LATER, doesn't work without it

		executeCmd(args);

	} 


	detachShmem(shPtr);

	return 0;
}

