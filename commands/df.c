#include <stdio.h>
#include "cmdSupport.h"

#define TOTAL_CLUSTERS 2880

int main(int argc, char *argv)
{
    char *fatEntry;
    int clusterCount, i, currentEntry;
    double percentage;
    
    if(argc != 1)
    {
        printf("This program doesn't take any arguments.");
        exit(-1);
    }
    
    fatEntry = readFAT12Table(1);
    
    for(i=1; i <= TOTAL_CLUSTERS; i++)
    {
        currentEntry = get_fat_entry(i, fatEntry);
        
        if(currentEntry == 0x000)
        {
            clusterCount++;
        }
    }
    
    percentage = TOTAL_CLUSTERS / clusterCount;
    
    printf("512K-Blocks     Used    Avalable    Use \n");
    printf("%i      %i     %i     %f \n",TOTAL_CLUSTERS, (TOTAL_CLUSTERS - clusterCount), clusterCount, percentage);
    
    return 0;
}