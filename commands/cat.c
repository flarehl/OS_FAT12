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
		    numSector = CPATH.sectorNum; //for relative path handling
		FileData* entry;

		//translate to physical sec num
		if(CPATH.sectorNum == 0)
			numSector = 19;
		else
			numSector += 31;


		entryNames = parsePath(argv[1]);
		
		int j = 0;
		for(j = 0; j < getArgc(entryNames); j++)
		{
			if(strncmp(entryNames[j], ".", 1) != 0 && strncmp(entryNames[j], "..", 2) != 0)
				entryNames[j] = fileTranslate(entryNames[j]);
		}

		// if absolute path, set sector equal to 19, otherwise start where we are
		if(getArgc(entryNames) > 1 && strncmp(entryNames[0], ".", 1) != 0 && strncmp(entryNames[0], "..", 2) != 0)
		{
			numSector = 19;
		}
		else if(getArgc(entryNames) == 1 && (strncmp(entryNames[0], ".", 1) == 0 || strncmp(entryNames[0], "..", 2) == 0))
			numSector = CPATH.sectorNum + 31;



		while( i < getArgc(entryNames) )
		{

			if((entry = searchEntries(entryNames[i], numSector)) == NULL)
			{
				printf("specified path does not exist\n");
				return -1;
			}

			if(entry->flc == 0)
				numSector = 19;
			else
				numSector = entry->flc + 31;

			i++;
		}

		if(isFile(entry))
		{
			
			FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
			if (FILE_SYSTEM_ID == NULL) 
			{
			  printf("Could not open the floppy drive or image.\n");
			  exit(1);
			}

	
			int numLoop = (entry->fileSize/BYTES_PER_SECTOR) + 1;
			unsigned int fatEntryNum = entry->flc;
			char *buffer = (char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
			unsigned char* fat;

			if(entry->fileSize > 512)
				fat = readFAT12Table(1);
			
			int k;
			for(k = 0; k < numLoop; k++)
			{

				memset(buffer, '\0', BYTES_PER_SECTOR);

				if (read_sector(numSector, buffer) == -1) 
				{
					printf("Something has gone wrong -- could not read the sector\n");
					exit(1);
				}
				else
				{
					printf("%s\n", buffer);
					if(entry->fileSize > 512)
					{
						fatEntryNum = get_fat_entry(fatEntryNum, fat); 
						numSector = fatEntryNum + 31;
					}
				}

			}

			return 0;

		}
		else 
		{
			printf("entry must be a file\n");
			return -1;
		}

	}
	else 
	{ 

		printf("wrong number of arguments for cat\n");
		return -1;
	}

	return 0;
}