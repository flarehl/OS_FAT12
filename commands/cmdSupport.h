#ifndef F_SUPPORT_H
#define F_SUPPORT_H

#define DEFAULT_BUF_SIZE 50

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../utilities/shmem.h"
#include "../structures/bootSector.h"
#include "../structures/fileData.h"

BootSector BOOT_SECTOR_ATTRIBUTES;
FILE* FILE_SYSTEM_ID; //just declare, cant initialize outside main

extern int BYTES_PER_SECTOR;

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

int read_sector(int sector_number, unsigned char* buffer);
int write_sector(int sector_number, unsigned char* buffer);


/* Functions added by HLeo & SBrikiatis */

void           displayLs(FileData*);
bool           extendDirectory(int); //work in progress
char*          fileTranslate(char*);
int            findFree(unsigned char*); 
int            getArgc(char**);
int            getPhysSector(int);
int            getSectorOffset(char *itemName, unsigned char *directory);
bool           isDeleted(FileData*);
bool           isEmpty(FileData*);
bool           isFile(FileData*);
bool           isFull(FileData*);
bool           isLongFile(FileData*);
bool           isReserved(FileData*);
char**         parsePath(char*);
FileData*      readEntry(char*, int*);
unsigned char* readFAT12Table(int FAT_Number);
FileData*      searchEntries(char*, int);

#endif
