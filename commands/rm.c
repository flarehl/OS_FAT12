#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char rDir[] = "root/";
    char **parsedPath;
    unsigned char *fat;
    FileData directory;
    int currentSectorNum;
    unsigned int fatEntryNumber;
    
    accessShmem(&shPtr); //accessing shared memeory struct
    
    memset(CPATH.path, '\0', MAX_PATH); //CPATH is in shmem.h and MAX_PATH is in cwd.h
    //this sets the name of the path
    
    memcpy(&CPATH, shPtr, SHMEMSIZE) //sets the path int
    
    FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+"); //the variable is in cmdSupport.h
    if(argc < 2)//check if there is the right number of items.
    {
        printf("Too few arguments for this command.\n");
        exit(-1);
    }
    else if(argc > 2)
    {
        printf("Too many items in the input for this command.\n");
        exit(-1);
    }
    
    
    fat = readFAT12Table(1);
    
    /*directory = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(sector_open(CPATH.sectorNum, directory) == -1); //opening the user's current directory
    {
        printf("There was a problem opening your directory.\n");
        exit(-1);
    }
    */
    
    directory = searchEntries(argv[1], CPATH.sectorNum);
    
    if(directory.fileName[0] == " ")
    {
        fatEntryNumber = get_fat_entry(directory.flc, fat);
        if(fatEntryNumber != 0xFF)
        {
            currentSectorNum 
            
        }
        
    }
    else
    {
        printf("The file you entered doesn't exist.");
        exit(0);
    }
    
    return 0;
}