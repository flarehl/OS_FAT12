#include "cmdSupport.h"

// PROTOTYPES //
bool addDir(char**);
bool validateInput(char**);


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
			if(parsed == NULL)
				addDir(entryNames);
			else
				addDir(parsed);
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

bool addDir(char **entryNames)
{
	int numSector = CPATH.sectorNum;
	FileData* entry, *entryBefore;

	//translate to physical sec num
	if(CPATH.sectorNum == 0)
		numSector = 19;
	else
		numSector += 31;

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
			
			/***** EXTENDDIRECTORY() NOT FUNCTIONAL YET *****
			if( isFull(entryBefore) )
			{
				// reallocate space for another sector if unreserved is available
				if(!extendDirectory(entryBefore->flc))
				{
					printf("directory could not be extended\n");
					return -1;
				}
				
			}
			*/

			unsigned char* buffer = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
			if(read_sector(numSector, buffer) == -1)
			{
				printf("Error reading sector %i\n", numSector);
				return -1;
			}

			int offset = findFree(buffer);
			int iOff = offset;

			// set filename
			int j = 0;
			for(i = offset; i < iOff + 8; i++)
			{
				if(j < strlen(entryNames[getArgc(entryNames) - 1]))
				{
					buffer[i] = entryNames[getArgc(entryNames) - 1][j];
					j++;
				}
				else
				{
					buffer[i] = (char)0x20;	
				}
			}

			iOff += 8;

			// set extension only deals with non ext name for testing
			for(; i < iOff + 3; i++)
			{
				buffer[i] = (char)0x20;
			}
			
			// set attribute to subdir
			buffer[i] |= (1 << 4);

			// skip unnecessary values
			i += 14;
			iOff += 14;

			// set flc to FREE FAT ENTRY NUMBER
			buffer[i++] = (numSector << 8) & 0xFF;
			buffer[i++] = numSector & 0xFF;

			// set filesize to 0 always 0 BROKEN
			buffer[i++] = 0x00;
			buffer[i++] = 0x00;
			buffer[i++] = 0x00;
			buffer[i++] = 0x00;
					


			    /**** for testing purposes ****/
			entry = readEntry(buffer, &offset);

			printf("filename: %s.\n", entry->fileName);
			printf("filesize: %i.\n", entry->fileSize);
			printf("flc:      %i.\n", entry->flc);
			printf("isfile:   %i.\n", isFile(entry));
			printf("fileext:  %s.\n", entry->fileExt);
			    /* end testing snippet */

			write_sector(numSector, buffer);

			printf("Successfully created directory\n");

			return 0;

		}
		else if((entry = searchEntries(entryNames[i], numSector)) != NULL && i != (getArgc(entryNames) - 1))
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

bool validateInput(char** argv)
{

}