#include "fatSupport.h"

void pfe(int first, int second) {
	unsigned char* fatBuffer;

	if (checkRange(first, second) ) {

		fatBuffer = readFAT12Table(1);

		for(int i = first; i < second; i++)
			printf( "%x\n", get_fat_entry(i, fatBuffer) );

	}
	else
		printf("%s\n", "Error: Invalid arguments\n");

}