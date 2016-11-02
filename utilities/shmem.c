#include "./shmem.h"

//double pointer because argument passed in is reference to pointer
bool accessShmem(void **ptr){

	int shmid; //need to initialize?
	char *shPtr;

	//find shared mem
	shmid = shmget(SHMEMKEY, SHMEMSIZE, 0644);
	if( shmid < 0 ){
		perror("shmget");
		return FALSE;
	};

	// attach shared mem
	shPtr = shmat(shmid, NULL, 0);
	if( (long)shPtr == -1 ){
		perror("shmat");
		return FALSE;
	};

	//if ptr is ptr != NULL
	if(ptr){
		*ptr = shPtr;
	}

	return TRUE;

}



bool createShmem(void **ptr){

	// SHARED MEMORY INITIALIZATION FOR PATH VARIABLE IN CD *
	int shmid; //need to initialize?
	char *shPtr;

	//create shared mem w/ error check
	shmid = shmget(SHMEMKEY, SHMEMSIZE, 0644 | IPC_CREAT);
	if( shmid < 0 ){
		perror("main shmget");
		return FALSE;
	};

	// attach shared mem
	shPtr = shmat(shmid, NULL, 0);
	if( (long)shPtr == -1 ){
		perror("main shmat");
		return FALSE;
	};

	//if ptr is ptr != NULL
	if(ptr){
		*ptr = shPtr;
	}

	return TRUE;
}



bool detachShmem(void *ptr){

	if (shmdt(ptr) < 0) {
		printf("Error detaching shared memory\n");
		return FALSE;
	}

	return TRUE;

}