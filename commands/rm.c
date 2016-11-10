#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char rDir[] = "root/";
    char **parsedPath;
    unsigned char *fat;
    int fatEntryNumber;
    
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
    
    
    fat = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(sector_open(1, fat) == -1)
    {
        printf("There was a problem opening the fat.");
        exit(-1);
    }
    
    return 0;
}