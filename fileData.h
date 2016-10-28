#include <stdlib.h>

#ifndef FILE_DATA_H
#define FILE_DATA_H


struct FileData{

	char fileName[9];
	char fileExt[4];
	int fileAttributes;
	short reserved; //??
	char createTime[3];
	char createDate[3];
	char lastAccessDate[3];
	char lastWriteTime[3];
	char lastWriteDate[3];
	short firstLogicalCluster; // 2 byte returns FAT index
	long fileSize; // 4 bytes

	struct FileData* next;
};


#endif