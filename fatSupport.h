#ifndef F_SUPPORT_H
#define F_SUPPORT_H

typedef int bool;

#define TRUE 1
#define FALSE 0

/* in alphabetical order but not yet in the fatSUpport.c file*/

bool checkRange(int x, int y);

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);

void printBootSector(); // last on fatSupport.c

unsigned char* readFAT12Table(int sectorNumber);
int read_sector(int sector_number, unsigned char* buffer);
int readBootSector();

void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

int write_sector(int sector_number, unsigned char* buffer);




#endif