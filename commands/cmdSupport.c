/******************************************************************************
 * Supporting functions for the FAT project:
 *  
 * Authors: Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *          March, 2004.
 * Modified by Hanah Leo
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "cmdSupport.h"

/******************************************************************************
 * FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                   filesystem)
 * BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 * BOOT_SECTOR_ATTRIBUTES -- a structure holding boot sector values
 *****************************************************************************/
extern FILE* FILE_SYSTEM_ID;
extern int BYTES_PER_SECTOR;
extern BootSector BOOT_SECTOR_ATTRIBUTES;

int BYTES_PER_SECTOR = 512; 


/******************************************************************************
 * read_sector
 *
 * sector_number:  The number of the sector to read (0, 1, 2, ...)
 * buffer:  The array into which to store the contents of the sector that is
 *          read
 *
 * Return: the number of bytes read, or -1 if the read fails.
 *****************************************************************************/
int read_sector(int sector_number, unsigned char* buffer)
{
   int bytes_read;

   if (fseek(FILE_SYSTEM_ID, (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0)
   {
	   printf("Error accessing sector %d\n", sector_number);
      return -1;
   }

   bytes_read = fread(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

   if (bytes_read != BYTES_PER_SECTOR)
   {
      printf("Error reading sector %d\n", sector_number);
      return -1;
   }

   return bytes_read;
}


/*****************************************************************************
 * write_sector
 *
 * Write the contents of the given buffer to the filesystem at the specified
 * sector
 *
 * sector_number:  The number of the sector to write (0, 1, 2, ...)
 * buffer:  The array whose contents are to be written
 *
 * Return: the number of bytes written, or -1 if the read fails.
 ****************************************************************************/
int write_sector(int sector_number, unsigned char* buffer) 
{
   int bytes_written;

   if (fseek(FILE_SYSTEM_ID,
       (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0) 
   {
      printf("Error accessing sector %d\n", sector_number);
      return -1;
   }

   bytes_written = fwrite(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

   if (bytes_written != BYTES_PER_SECTOR) 
   {
      printf("Error reading sector %d\n", sector_number);
      return -1;
   }

   return bytes_written;
}



/*****************************************************************************
 * get_fat_entry
 *
 * Get the specified entry from the given FAT
 *
 * fat_entry_number:  The number of the FAT entry to get (0, 1, 2, ...)
 * fat:  The fat table from which to get the specified entry
 *
 * printf("Directory listing returned %d entries:\n", dnum);
 * Return: the value at the specified entry of the given FAT
 ****************************************************************************/

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat) 
{
   int offset;
   int uv, wx, yz;

   offset = 3 * fat_entry_number / 2;

   // Two FAT12 entries are stored into three bytes;
   // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw

   // odd fat entry number, return yzw
   if (fat_entry_number & 0x0001) {
      wx = (int) fat[offset];
      yz = (int) fat[offset + 1];
      return ( (yz << 4)  |  ( (wx & 0x00f0) >> 4) );
   } 
   // even fat entry number, return xuv
   else {
      uv = (int) fat[offset];
      wx = (int) fat[offset + 1];
      return ( ((wx & 0x000f) << 8)  |  uv );
   }
}



/******************************************************************************
 * set_fat_entry
 *
 * Set the specified entry in the given FAT to the given value
 *
 * fat_entry_number:  The number of the FAT entry to set (0, 1, 2, ...)
 * value:  The given value to place in the FAT entry
 * fat:  The fat table in which to set the given value at the specified entry
 *****************************************************************************/
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat) 
{
   int offset;
   int uv, wx, yz, a, b, c;

   offset = 3 * fat_entry_number / 2;

   // Two FAT12 entries are stored into three bytes;
   // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw
   // Let 0a,bc denote the fat_entry_number, written as two bytes (high and
   // low, respectively)

   a = value & 0x0f00;
   b = value & 0x00f0;
   c = value & 0x000f;

   // odd fat entry number, change yzw to abc, i.e.,
   if (fat_entry_number & 0x0001) 
   {
      // wx = cx;
      fat[offset]     = (unsigned char) ((c << 4)  |  (fat[offset] & 0x000f));
      // yz = ab;
      fat[offset + 1] = (unsigned char) ((a >> 4)  |  (b >> 4));
   }
   // even fat entry number, change xuv to abc, i.e.,
   else
   {
      // uv = bc;
      fat[offset]     = (unsigned char) (b | c);
      // wx = wa;
      fat[offset + 1] = (unsigned char) ((fat[offset + 1]  & 
                                          0x00f0)  |  (a >> 8));
   }
}


/******************************************************************************
* readEntry
*
* reads and sets entry attributes
*
* buffer:  buffer holding entries of a sectoor
* offset:  where from buffer to read 
*
* Return: FileData object holding entry info
*****************************************************************************/
FileData readEntry(char* buffer, int *offset){

   FileData entry;

   for(int i = 0; i < 8 || buffer[*offset] != '\0'; i++){
      entry.fileName[i] = buffer[*offset];
      (*offset)++;
   }

   /* TEST */ printf("offset after name %d\n", *offset);

   for(int i = 0; i < 3; i++){
      entry.fileExt[i] = buffer[*offset];
      (*offset)++;
   }

   entry.fileAttributes = buffer[*offset];
   (*offset)++;

   /* TEST */ printf("offsets @ reserved: %d\n", *offset);

   short mostSignificantBits = (((short)buffer[*offset+1]) << 8) & 0xff00;
   short leastSignificantBits = ((short)buffer[*offset]) & 0x00ff;
   entry.reserved = mostSignificantBits | leastSignificantBits;

   *offset += 2;

   for(int i = 0; i < 2; i++){
      entry.createTime[i] = buffer[*offset];
      (*offset)++;

   }

   for(int i = 0; i < 2; i++){
      entry.createDate[i] = buffer[*offset];
      (*offset)++;
   }  

   for(int i = 0; i < 2; i++){
      entry.lastAccessDate[i] = buffer[*offset];
      (*offset)++;
   }

   for(int i = 0; i < 2; i++){
      entry.lastWriteTime[i] = buffer[*offset];
      (*offset)++;
   }


   for(int i = 0; i < 2; i++){
      entry.lastWriteDate[i] = buffer[*offset];
      (*offset)++;
   }

   mostSignificantBits = (((short)buffer[*offset+1]) << 8) & 0xff00;
   leastSignificantBits = ((short)buffer[*offset]) & 0x00ff;
   entry.flc = mostSignificantBits | leastSignificantBits;

   *offset += 2;

   mostSignificantBits = (((int)buffer[*offset+3]) << 24) & 0xff000000;
   int midBits = ((int)buffer[*offset+2] << 16) & 0x00ff0000;
   int midBits2 = ((int)buffer[*offset+1] << 8) & 0x0000ff00;
   leastSignificantBits = ((int)buffer[*offset]) & 0x000000ff;
   entry.fileSize = (mostSignificantBits | midBits | midBits2 | leastSignificantBits) ;

   return entry;
}

/******************************************************************************
* parsePath
*
* slash delimits path so filenames can be directly compared
*
* path: path argument passed into command
*
* Return: slash delimited "argv"
*****************************************************************************/
char** parsePath(char* path){

   //quotes should be stripped by parseInput already if they were included
   char slashDelim[] = "\\", *token;
   char** args = (char**)malloc(DEFAULT_BUF_SIZE * sizeof(char));
   int location = 0,
      bufSize = DEFAULT_BUF_SIZE;

   token = strtok(path, slashDelim);
   if( token == NULL )
      printf("Error slash delimiting\n");

   // loop through until all tokens are handed off to parsedInput
   while(token!= NULL){

      args[location] = token;
      location++;

      // if the buffer is too small, reallocate space in 50byte increments
      if(location >= bufSize){

         bufSize += DEFAULT_BUF_SIZE;
         args = (char**)realloc(args, bufSize * sizeof(char*));

         //maybe add error handling for reallocation of parsedInput
      }

      token = strtok(NULL, slashDelim);
   }

   args[location] = NULL; //set null terminating char for args
      
   return args;

}
