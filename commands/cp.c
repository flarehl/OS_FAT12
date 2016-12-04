#include <stdio.h>
#include "cmdSupport.h"

int main(int argc, char **argv)
{
    void *shPtr;
    char *parsedInput1, *parsedInput2;
    unsigned char *sector, *tempSector, *fat, sectorList[100]; //These are a full 16 item sector
    int location, freelocation, sizeOfItem=0, currentFatItem, i;
    FileData itemSector, newFileData; //This si one item in a given sector with it's info
    bool foundSpace = FALSE;
    
    if(argc != 3)
    {
        printf("Wrong number of inputs.");
        exit(-1);
    }
    
    accessShmem(&shPtr);
    memset(CPATH.path, '\0', MAX_PATH);
    
    FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
    
    if(CPATH.sectorNum == 0)
    {
        location = 19;
    }
    else
    {
        location = CPATH.sectorNum + 31;
    }
    
    sector = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    tempSector = (unsigned char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char*));
    fat = readFAT12Table(1);
    
    if(read_sector((int)location, sector) == -1)
    {
        printf("There was a problem opening this sector.");
        exit(-1);
    }
    
    parsedInput1 = fileTranslate(argv[1]);
    parsedInput2 = fileTranslate(argv[2]);
    
    itemSector = searchEntries(parsedInput1, location);
    
    currentFatItem = get_fat_entry(itemSector.flc, fat);
    sizeOfItem++;
    
    while(currentFatItem != 0xFFF)
    {
        currentFatItem = get_fat_entry(currentFatItem, fat);
        sizeOfItem++;
    }
    
    for(i = 0; i < 16; i++)
    {
        if(sector[i * 0x20] == 0x00 || sector[i * 0x20] == 0xE5)
        {
            foundSpace = TRUE;
            break;
        }
    }
    
    
    if(foundSpace == TRUE)
    {
        for(i = 0; i < sizeOfItem; i++)
        {
             sectorList[i] = findFreeCluster();
             writeToFat(sectorList[i], 0x001);
        }

        sectorList[sizeOfItem] = 0xFFF;

        for(i = 0; i < sizeOfItem; i++)
        {
            writeToFat(sectorList[i], sectorList[i+1]);
        }
    }
    else
    {
        printf("Cannot write to this directory.");
    }
    
    newFileData.fileName = ;
    
    for(i = 0; i < sizeOfItem; i++)
    {
        
    }
    
    return 0;
}