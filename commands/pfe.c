#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cmdSupport.h"

//prototypes
bool checkRange(int x, int y);

//pfe program
int main(int argc, char** argv) {

	unsigned char* fatBuffer;
	int i;

	if(argc != 3)
		printf("Error: pfe only accepts 2 arguments\n");

	if ( checkRange( atoi(argv[1]), atoi(argv[2]) ) ) 
	{

		fatBuffer = readFAT12Table(1);

		//display results
		for(i = atoi(argv[1]); i <= atoi(argv[2]); i++)
			printf( "Entry %d: %x\n", i, get_fat_entry(i, fatBuffer) );


	}
	else 
	{
		printf("%s\n", "Error: Invalid argument(s)\n");
		exit(1);
	}		
		
	return 0;

}


/******************************************************************************
* checkRange
*
* User command argument validation
*
* x:  first sector value to begin read
* y:  second sector value to end at 
*
* Return: true or false depending on range
*****************************************************************************/
bool checkRange(int x, int y) {

	if (x < 2) {
		printf("%s\n", "Error: first sector cannot be below the value 2");
		return FALSE;
	}
	else if (x > y) {
		printf("%s\n", "Error: first sector value cannot be over the second sector value ");
		return FALSE;
	}
	else
		return TRUE;

}

