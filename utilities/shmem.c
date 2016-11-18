#include "./shmem.h"


/******************************************************************************
* accessShmem
*
* attaches the program to an existing shared memory segment
*
* ptr: the shared memory pointer that will be passed in by reference that holds
* 	the shared memory variables
*  
* Return: true if successfully attached, false otherwise
*****************************************************************************/
bool accessShmem(void **ptr)
{

	int shmid; 
	char *shPtr;

	//find shared mem
	shmid = shmget(SHMEMKEY, SHMEMSIZE, 0644);
	if( shmid < 0 ){
		perror("shmget");
		return FALSE;
	};

	// attach shared mem
	shPtr = shmat(shmid, NULL, 0);
	if( (long)shPtr == -1 )
	{
		perror("shmat");
		return FALSE;
	};

	//if ptr is ptr != NULL
	if(ptr)
		*ptr = shPtr;


	return TRUE;

}



/******************************************************************************
* createShmem
*
* creates the existing shared memory segment
*
* ptr: the shared memory pointer that will be passed in by reference that holds
* 	the shared memory variables
*  
* Return: true if successfully created, false otherwise
*****************************************************************************/
bool createShmem(void **ptr)
{

	int shmid; //need to initialize?
	char *shPtr;

	//create shared mem w/ error check
	shmid = shmget(SHMEMKEY, SHMEMSIZE, 0644 | IPC_CREAT);
	if( shmid < 0 )
	{
		perror("main shmget");
		return FALSE;
	};

	// attach shared mem
	shPtr = shmat(shmid, NULL, 0);
	if( (long)shPtr == -1 )
	{
		perror("main shmat");
		return FALSE;
	};

	//if ptr is ptr != NULL
	if(ptr)
		*ptr = shPtr;

	return TRUE;
}



/******************************************************************************
* detachShmem
*
* detaches the program from an existing shared memory segment
*
* ptr: the shared memory pointer that will be passed in by reference that holds
* 	the shared memory variables
*  
* Return: true if successfully detached, false otherwise
*****************************************************************************/
bool detachShmem(void *ptr)
{

	if (shmdt(ptr) < 0) 
	{
		printf("Error detaching shared memory\n");
		return FALSE;
	}

	return TRUE;

}