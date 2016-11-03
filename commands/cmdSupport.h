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


/*Functions added by HLeo */
int      getPhysSector(int);
bool     isAbsolutePath(char*);
bool     isFile(FileData entry);
bool     isRelativePath(char*);
char**   parsePath(char*);
FileData readEntry(char*, int*);
bool     searchEntries(char**);

#endif
