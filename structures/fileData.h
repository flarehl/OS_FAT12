#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <stdlib.h> 

typedef struct FileData{

	char fileName[9];
	char fileExt[4];
	char fileAttributes;
	short reserved; 
	char createTime[3];
	char createDate[3];
	char lastAccessDate[3];
	char lastWriteTime[3];
	char lastWriteDate[3];
	short flc; 
	int fileSize; 
} FileData;


#endif