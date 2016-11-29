#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <stdlib.h> 

typedef struct FileData{

	char fileName[9];
	char fileExt[4];
	char fileAttributes;
	short flc; 
	int fileSize;
	 
} FileData;


#endif