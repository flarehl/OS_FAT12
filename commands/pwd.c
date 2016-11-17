#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char** argv) {

	void *shPtr;
	CurrentPath cPath;

	if(argc != 1)
	{
		printf("wrong number of arguments\n");
		return -1;
	}
	
	accessShmem(&shPtr); //passing address of the pointer, int value
	memcpy(&cPath, shPtr, SHMEMSIZE); //read in from shared memory

	printf("%s\n", cPath.path);

	detachShmem(shPtr);

	return 0;
	
}
