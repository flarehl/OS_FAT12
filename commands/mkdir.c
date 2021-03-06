#include "cmdSupport.h"

// PROTOTYPES //
bool addDir(char**);
int createDir(int, char*, char*, int);

//*************** MKDIR IMPLEMENTATION W.I.P *******************//
int main(int argc, char **argv){
	
	/* Setup Shared Memory */
	void *shPtr;
	accessShmem(&shPtr); //passing address of the pointer, int value
	memset(CPATH.path, '\0', MAX_PATH);
	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory

	if(argc == 2)
	{
		char **entryNames = parsePath(argv[1]);
		char* string = (char*)malloc(SHMEMSIZE * sizeof(char));

		if(getArgc(entryNames) == 1)
		{
			strcpy(string, CPATH.path);
			strcat(string, argv[1]);	

			char **parsed = parsePath(string);

			if(parsed == NULL){
				addDir(entryNames);
			}
			else{
				addDir(parsed);
			}
			

		}
		else
		{
			addDir(entryNames);
		}

		return 0;

	}
	else 
	{ 

		printf("wrong number of arguments for mkdir\n");
		return -1;
	}

} /**** END MKDIR IMPLEMENTATION *****/



//******************* FUNCTION DEFINITIONS *********************//

/******************************************************************************
* addDir
*
* finds an unreserved entry in FAT
*  
* Return: logical cluster number, -1 if not found
*****************************************************************************/
bool addDir(char **entryNames)
{
	int numSector = 19; //everything DOES NOT start at root
	if(strcmp(entryNames[0], ".") == 0 || strcmp(entryNames[0], "..") == 0 ){
		numSector = CPATH.sectorNum + 31;
	}

	FileData* entry, *entryBefore;
	char *last = (char*)malloc(12 * sizeof(char));
	strcpy(last, entryNames[getArgc(entryNames) - 1]);

	// uppercase everything ignore extensions for now
	int j;
	int i;

	for(i = 0; i < getArgc(entryNames); i++)
	{
		for(j = 0; j < strlen(entryNames[i]); j++)
			entryNames[i][j] = toupper(entryNames[i][j]);

	}

	i = 0;
	while( i < getArgc(entryNames) )
	{

		if((entry = searchEntries(entryNames[i], numSector)) == NULL && i == (getArgc(entryNames) - 1) )
		{
			
			/******* EXTENDDIRECTORY() NOT FUNCTIONAL YET ********
			if( isFull(entryBefore) )
			{
				// reallocate space for another sector if unreserved is available
				if(!extendDirectory(entryBefore->flc))
				{
					printf("directory could not be extended\n");
					return -1;
				}
				
			}*/
			
			//validate the input that is to be used for the dir name
			if(validateEntryName(entryNames[getArgc(entryNames) - 1]))
			{
				unsigned char* buffer = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
				int nCluster = createDir(numSector, last, buffer, -1); 

				nCluster += 31;

				createDir(nCluster, ".", buffer, -1); // -1 is filler
				createDir(nCluster, "..", buffer, entryBefore->flc);	

				return 0;
			}
			else
				return -1;

		}
		else if((entry = searchEntries(entryNames[i], numSector)) != NULL && i < (getArgc(entryNames) - 1))
		{
			entryBefore = entry;

			if(entry->flc == 0)
				numSector = 19; //root multidir handling needed
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


/******************************************************************************
* createDir
*
* finds an unreserved entry in FAT
*  
* Return: sector of directory created
*****************************************************************************/
int createDir(int numSector, char* fname, char* buffer, int prevSec)
{
	if(read_sector(numSector, buffer) == -1)
	{
		printf("Error reading sector %i\n", numSector);
		return -1;
	}

	FileData *entry;
	char delim[2] = ".";
	char** tokens;
	int offset = findFree(buffer);
	int iOff = offset;
	int freeCluster;

	if(strcmp(fname, ".") != 0 || strcmp(fname, "..") != 0 )
	{	
		tokens[0] = strtok(fname, delim);
		tokens[1] = strtok(NULL, delim);
	}
	else
	{
		strcpy(tokens[0], fname);
		tokens[1] = NULL;
	}

	// set filename
	int i, j = 0;
	for(i = offset; i < iOff + 8; i++)
	{
		if(j < strlen(tokens[0]))
		{
			buffer[i] = (long)tokens[0][j];
			j++;
		}
		else
		{
			buffer[i] = (char)0x20;	
		}
	}
	iOff += 8;

	// set extension only deals with non ext name for testing
	j = 0;
	for(i = iOff; i < iOff + 3; i++)
	{
		if(tokens[1] != NULL && j < strlen(tokens[1]))
		{
			buffer[i] = (long)tokens[1][j];
			j++;
		}
		else
		{
			buffer[i] = (char)0x20;	
		}
	}
	
	// set attribute to subdir
	buffer[i] = (char)0x10;

	i += 15;

	// set flc
	if(strcmp(fname, ".") == 0)
	{
		numSector -= 31;
		buffer[i+1] = (numSector << 8) & 0xFF;
		buffer[i] = numSector & 0xFF;

		i += 2;

		numSector += 31;
		writeToFAT(numSector,(int)0xfff);
	}
	else if(strcmp(fname, "..") == 0)
	{
		buffer[i+1] = (prevSec << 8) & 0xFF;
		buffer[i] = prevSec & 0xFF;

		i += 2;

		writeToFAT(prevSec,(int)0xfff);
	}
	else
	{
		freeCluster = findFreeCluster();

		buffer[i+1] = (freeCluster << 8) & 0xFF;
		buffer[i] = freeCluster & 0xFF;

		i+=2; //increase one more time
		writeToFAT(freeCluster,(int)0xfff);

		// set new directory buffer so offset isnt fucked, DOESNT WORK
		unsigned char* nBuffer = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
		memset(nBuffer, 0, BYTES_PER_SECTOR);

		freeCluster += 31;
		write_sector(freeCluster, nBuffer);
		freeCluster -= 31;


	}

	i += 3;

	// set filesize
	buffer[i--] = 0;
	buffer[i--] = 0;
	buffer[i--] = 0;
	buffer[i] = 0;

	write_sector(numSector, buffer);
	return freeCluster;
}

