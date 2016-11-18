#include "cmdSupport.h"

int main(int argc, char**argv){
	
	/* Setup Shared Memory */
	void *shPtr;
	accessShmem(&shPtr); //passing address of the pointer, int value
	memset(CPATH.path, '\0', MAX_PATH);
	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory

	if(argc == 2)
	{

		char** entryNames;
		int i = 0,
		    numSector = CPATH.sectorNum;

		FileData* entry, *entryBefore;

		//translate to physical sec num
		if(CPATH.sectorNum == 0)
			numSector = 19;
		else
			numSector += 31;


		entryNames = parsePath(argv[1]);


		if( getArgc(entryNames) == 1 )
		{
			// search current CPATH and create mkdir there, meaning
			// there should be a function that deals with the actual creation

		}


		while( i < getArgc(entryNames) )
		{

			if((entry = searchEntries(entryNames[i], numSector)) == NULL && i == (getArgc(entryNames) - 1) )
			{
				
				if( isFull(entryBefore) )
				{
					// reallocate space for another sector if unreserved is available
					// otherwise see below
				}

				//otherwise find unreserved entry in FAT table
					//read FAT first or handle in findUnreserved??


				return 0;

			}
			else if((entry = searchEntries(entryNames[i], numSector)) != NULL && i != (getArgc(entryNames) - 1))
			{
				entryBefore = entry;

				if(entry->flc == 0)
					numSector = 19;
				else
					numSector = entry->flc + 31;
					
				i++;
			}
			else
			{
				printf("entry already exists\n");
				return -1;
			}

		}

	}
	else 
	{ 

		printf("wrong number of arguments for mkdir\n");
		return -1;
	}

	return 0;
}