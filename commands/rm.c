#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char rDir[] = "ROOT";
    char *parsedPath;
    unsigned char *fat, *directory;
    FileData *directorySector;
    int currentSectorNum;
    unsigned int fatEntryNumber;
    int offset;
    int location; //This is the actual sector number in memory
    
    accessShmem(&shPtr); //accessing shared memory struct
    memset(CPATH.path, '\0', MAX_PATH); //CPATH is in shmem.h and MAX_PATH is in cwd.h
    //this sets the name of the path
    
    memcpy(&CPATH, shPtr, SHMEMSIZE); //sets the path int
    
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
    
    if(CPATH.sectorNum == 0)
    {
        location = 19;
    }
    else
    {
        location = CPATH.sectorNum + 31;
    }
    
    parsedPath = fileTranslate(argv[1]);
    
    fat = readFAT12Table(1);
    
    directory = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    if(read_sector((int)location, directory) == -1) //opening the user's current directory
    {
        printf("There was a problem opening your directory.\n");
        exit(-1);
    }
    
    
    directorySector = searchEntries(parsedPath, location);
    
    //currently only reads the first sector of a folder, which will work, but
    //its really not complete for the program
    if(directorySector->fileName[0] != 0x00)
    {
        fatEntryNumber = get_fat_entry(directorySector->flc, fat);
        printf("Debug directorySec: %u\n", directorySector->flc);
        writeToFAT(directorySector->flc); //these need to have a entry type added to them
        while(fatEntryNumber != 0xFFF)
        {
            currentSectorNum = get_fat_entry(fatEntryNumber, fat);
            writeToFAT(fatEntryNumber);
            fatEntryNumber = currentSectorNum;
        }
        
        offset = getSectorOffset(parsedPath, directory); //gets the actaul offset of
        //the location
        if(offset == -1)
        {
            printf("rm had problems opening the directory entry.");
        }
        
        directory[offset] = 0xE5;
        
        write_sector(location, directory);
    }
    else
    {
        printf("The file you entered doesn't exist.");
        exit(0);
    }
    
    return 0;
}
