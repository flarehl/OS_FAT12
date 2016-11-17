#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char rDir[] = "root/";
    char *parsedPath;
    unsigned char *fat, directory;
    FileData directorySector;
    int currentSectorNum;
    unsigned int fatEntryNumber;
    int offset;
    
    accessShmem(&shPtr);
    
    memset(CPATH.path, '\0', MAX_PATH);
    
    memcpy(&CPATH, shPtr, SHMEMSIZE);
    
    FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
    if(argc < 2)
    {
        printf("Too few arguments for this program.");
        exit(-1);
    }
    else if (argc > 2)
    {
        printf("Too many arguments for this program.");
    }
    
    fat = readFATTable(1);
    
    directory = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(read_sector((int)CPATH.path, directory) == -1)
    {
        printf("There was a problem opening your directory.");
        exit(-1);
    }
    
    directorySector = searchEntries(parsedPath, CPATH.sectorNum);
    
    if(directorySector->fileName[0] != 0x00)
    {
        fatEntryNumber = get_fat_entry(directorySector->flc, fat);
        
    }
    
    return 0;
}