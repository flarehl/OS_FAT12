#include <stdio.h>
#include "cmdSupport.h"

#define TOTAL_CLUSTERS 2880

int main(int argc, char *argv)
{
    char *fatEntry;
    double clusterCount =0, currentEntry =0;
    double percentage =0;
    int i;
    
    if(argc != 1)
    {
        printf("This program doesn't take any arguments.");
        exit(-1);
    }
    
    fatEntry = readFAT12Table(1); //called form cmdSupppot.c
    
    for(i=1; i <= TOTAL_CLUSTERS; i++)
    {
        currentEntry = get_fat_entry(i, fatEntry);
        
        if(currentEntry == 0x000) //This is the only entry type for an empty sector
        {
            clusterCount++;
        }
    }
    
    percentage = (TOTAL_CLUSTERS - clusterCount) / TOTAL_CLUSTERS;
    
    printf("512K-Blocks     Used    Available    Use %% \n");
    printf("%8i %3.0f %3.0f %3.2f \n",TOTAL_CLUSTERS, (TOTAL_CLUSTERS - clusterCount), clusterCount, percentage);
    
    return 0;
}