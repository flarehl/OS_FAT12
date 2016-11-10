#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char rDir[] = "root/";
    char **parsedPath;
    unsigned char *fat;
    unsigned char *directory;
    int fatEntryNumber;
    
    accessShmem(&shPtr); //accessing shared memeory struct
    
    memset(CPATH.path, '\0', MAX_PATH); //CPATH is in shmem.h and MAX_PATH is in cwd.h
    //this sets the name of the path
    
    memcpy(&CPATH, shPtr, SHMEMSIZE) //sets the path int
    
    FILE_SYSTEM_ID = fopen("./floppies/floppy1", "r+"); //the variable is in cmdSupport.h
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
    
    
    fat = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(sector_open(1, fat) == -1) //opening the first fat directory
    {
        printf("There was a problem opening the fat.\n");
        exit(-1);
    }
    
    directory = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(sector_open(CPATH.sectorNum, directory) == -1); //opening the user's current directory
    {
        printf("There was a problem opening your directory.\n");
        exit(-1);
    }
    
    if(itemExists(argv[1], directory) == TRUE)
    {
        
    }
    else
    {
        printf("The file you entered doesn't exist.");
        exit(0);
    }
    
    return 0;
}