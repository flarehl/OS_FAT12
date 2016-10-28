#ifndef F_SUPPORT_H
#define F_SUPPORT_H

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "bootSector.h"
#include "fileData.h"

#define DEFAULT_BUF_SIZE 50

BootSector BOOT_SECTOR_ATTRIBUTES;
FILE* FILE_SYSTEM_ID; //just declare, cant initialize outside main
struct FileData PATH;

extern int BYTES_PER_SECTOR;

typedef int bool;

#define TRUE 1
#define FALSE 0

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

int read_sector(int sector_number, unsigned char* buffer);
int write_sector(int sector_number, unsigned char* buffer);


/* start alphabetically ordered commands here */ 
void executeCmd(char** args);
char* getInput();
char** parseInput(char*);
void printBootSector(); 
int readBootSector();


#endif
