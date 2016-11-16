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








/* Additional Functions by HL, SB */




/******************************************************************************
* getPhysSector
*
* translates logical sector number to physical sector number
*
* logicalNum: sector number to be translated
*
* Return: physical sector num
*****************************************************************************/
int getPhysSector(int logicalNum){
   return logicalNum + 31;
}


/******************************************************************************
* isAbsolutePath
*
* checks if the argument provided is an absolute path
*
* path: first file name
*
* Return: true if absolute path
*****************************************************************************/
bool isAbsolutePath(char* path){
   if( strncmp(path, "root", 4) == 0 )
      return TRUE;
   else
      return FALSE;
}

/******************************************************************************
* isDeleted
*
* checks if file entry is deleted
*
* entry: file entry to be checked
*
* Return: true if the entry has been deleted, return false otherwise
*****************************************************************************/
bool isDeleted(FileData *entry){

   if(entry->fileName[0] == (char)0xE5)
      return TRUE;
   else
      return FALSE;

}


/******************************************************************************
* isEmpty
*
* checks if file entry is empty 
*
* entry: file entry to be checked
*
* Return: true if entry is not empty, return false otherwise
*****************************************************************************/
bool isEmpty(FileData *entry){

   if (entry->fileName[0] == (char)0x00 )
      return TRUE; 
   else
      return FALSE;

}


/******************************************************************************
* isFile
*
* checks if file entry is a file or a subdirectory by masking attributes
* and seeing if 5th bit is on or off
*
* entry: file entry to be checked
*
* Return: true if entry is a file, return false otherwise
*****************************************************************************/
bool isFile(FileData *entry){

   if ((entry->fileAttributes & (char)0x10 ) == (char)0x10 )
      return FALSE; 
   else
      return TRUE;

}


/******************************************************************************
* isLongFile
*
* Checks if file entry needs to be ignored
*
* entry:  file entry with name to be checked
*
* Return: true if file entry should be ignored
*****************************************************************************/
bool isLongFile(FileData *entry){

   if(entry->fileAttributes == 0x0f)
      return TRUE;
   else 
      return FALSE;
}


/******************************************************************************
* isRelativePath
*
* checks if the argument provided is a relative path
*
* path: first file name
*
* Return: return true if relative path
*****************************************************************************/
bool isRelativePath(char* path){
   if( strncmp(path, "./", 2) || strncmp(path, "../", 3) )
      return TRUE; //add isExist()
   else
      return FALSE;
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
   char slashDelim[] = "/", *token;
   char** args = (char**)malloc(DEFAULT_BUF_SIZE * sizeof(char));
   int location = 0,
      bufSize = DEFAULT_BUF_SIZE;

   token = strtok(path, slashDelim);
   if( token == NULL )
      printf("Error slash delimiting\n");

   // loop through until all tokens are handed off to parsedInput
   while(token!= NULL){

      for(int i = 0; i < strlen(token); i++)
         token[i] = toupper(token[i]);

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
FileData* readEntry(char* buffer, int *offset){

   FileData* entry;
   entry = (FileData*)malloc(sizeof(FileData));

   int i;
   for(i = 0; i < 8; i++){
      entry->fileName[i] = buffer[*offset];
      (*offset)++;
   }
   entry->fileName[i] = '\0';

   for(i = 0; i < 3; i++){
      entry->fileExt[i] = buffer[*offset];          
      (*offset)++;
   }
   entry->fileExt[i] = '\0';

   entry->fileAttributes = buffer[*offset];
   (*offset)++;

   short mostSignificantBits = (((short)buffer[*offset+1]) << 8) & 0xff00;
   short leastSignificantBits = ((short)buffer[*offset]) & 0x00ff;
   entry->reserved = mostSignificantBits | leastSignificantBits;

   *offset += 2;

   for(i = 0; i < 2; i++){
      entry->createTime[i] = buffer[*offset];
      (*offset)++;
   }
   entry->createTime[i] = '\0';

   for(i = 0; i < 2; i++){
      entry->createDate[i] = buffer[*offset];
      (*offset)++;
   }
   entry->createDate[i] = '\0';  

   for(i = 0; i < 2; i++){
      entry->lastAccessDate[i] = buffer[*offset];
      (*offset)++;
   }
   entry->lastAccessDate[i] = '\0';

   //ignore 2 bytes here
   (*offset)+=2;

   for(i = 0; i < 2; i++){
      entry->lastWriteTime[i] = buffer[*offset];
      (*offset)++;
   }
   entry->lastWriteTime[i] = '\0';


   for(i = 0; i < 2; i++){
      entry->lastWriteDate[i] = buffer[*offset];
      (*offset)++;
   }
   entry->lastWriteDate[i] = '\0';

   mostSignificantBits = (((short)buffer[*offset+1]) << 8) & 0xff00;
   leastSignificantBits = ((short)buffer[*offset]) & 0x00ff;
   entry->flc = mostSignificantBits | leastSignificantBits;
   *offset += 2;

   mostSignificantBits = (((int)buffer[*offset+3]) << 24) & 0xff000000;
   int midBits = ((int)buffer[*offset+2] << 16) & 0x00ff0000;
   int midBits2 = ((int)buffer[*offset+1] << 8) & 0x0000ff00;
   leastSignificantBits = ((int)buffer[*offset]) & 0x000000ff;
   entry->fileSize = (mostSignificantBits | midBits | midBits2 | leastSignificantBits) ;

   *offset += 4;

   return entry;
}



/******************************************************************************
* readFAT12Table
*
* Reads the FAT12 tables 
*
* sectorNumber:  The number of the FAT entry
*
* return: returns buffer holding fat entries
*****************************************************************************/
unsigned char* readFAT12Table(int FAT_Number) {

   unsigned char* fat = (unsigned char*)malloc( 9 * BYTES_PER_SECTOR * sizeof(unsigned char) );

   FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");

   if (FILE_SYSTEM_ID == NULL)
   {
      printf("Could not open the floppy drive or image.\n");
      exit(1);
   }

   // i is 0-numSectors, fat holds the data
   if (FAT_Number == 1) {
      for (int i = 0; i < 9; i++) 
         read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
      return fat;
   }
   else if (FAT_Number == 2) {
      for (int i = 10; i < 19; i++)
         read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
      return fat;
   }
   else
      printf("%s\n", "Error: invalid FAT table number");
   

}


/******************************************************************************
* searchEntries
*
* searches for the entry
*
* fileName: the file name to be searched for
* sectorNumber: number of the current sector to be read
*
* Return: if entry does not exist, returns FileData object with blank 
*         file name
*****************************************************************************/
FileData* searchEntries(char* fileName, int sectorNumber){
   
   //* TEST */printf("sectorNum: %i.\n", sectorNumber);

   FileData *nEntry; //used as empty value
   FileData *entry;
   int offset = 0;

   FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
   if (FILE_SYSTEM_ID == NULL) {
      printf("Could not open the floppy drive or image.\n");
      exit(1);
   }

   //read in the appropriate sector set by sectorNumber
   char *buffer = (char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
   if (read_sector(sectorNumber, buffer) == -1) {
      printf("Something has gone wrong -- could not read the sector\n");
      exit(1);
   }

   if( strcmp(fileName, ".") != 0 && strcmp(fileName, "..") != 0){
      fileName = fileTranslate(fileName);
   }

   /*put in do while loop to deal with directories spanning multiple sectors*/

      for(int i = 0; i < 16; i++){ // 16 entries per sector

         //reaches here 
         entry = nEntry; //to reset values, spaghettiiiiiii

         if( (entry = readEntry(buffer, &offset)) == NULL){
            printf("Cannot read entry\n");
            break;
         }

         if( entry->fileName[0] == (char)0x00 ){
            break; //if empty, break
         }
         else if( entry->fileAttributes == (char)0x0f || entry->fileName[0] == (char)0xE5 ){
            continue;

         }else{

            char* name = strtok(entry->fileName, " ");

            if(entry->fileExt[0] != ' '){
               char space[] = " ";
               char *ext;

               ext = strtok(entry->fileExt, space);
               strcat(name, space);
               strcat(name, ext);
            }
  
            if( strcmp(name, fileName) == 0 )
               return entry; 

         }

      }

   return NULL;

}


/******************************************************************************
* fileTranslate
*
* translates given filename to all caps and replaces any dots with spaces
*
* fileName: the file name to be translated
*
* Return: if entry does not exist, returns FileData object with blank 
*         file name
*****************************************************************************/
char* fileTranslate(char* fileName){

   for(int i = 0; i < strlen(fileName); i++){
      fileName[i] = toupper(fileName[i]);
      if(fileName[i] == '.')
         fileName[i] = ' '; //replace dots with spaces
   }

   return fileName;

}


/******************************************************************************
* itemExists
*
* compare's user input with the current sector to find if a file or directory
* exists
*
* itemName: the name of the file or directory that is being searched for
* directory: the current sector of the directory being searched
*  
* Return: the found sectors number or -1 if not found
*****************************************************************************/
int itemExists(char *itemName, unsigned char *directory)
{
    char currentItemName[12];
    char holder;
    int currentOffset = 0x00;
    bool fileExists = FALSE;
    int i, j;
    int currentItemNameSize; //needed to keep track of the actaul size of the
        //item being checked
    

    for(i = 0; i < 16; i++)
    {
        if(fileExists == FALSE)
        {
            for(j = 0; j < 8; j++) //This loop gets the file name
            {
               holder = (char ) directory[currentOffset +j];
                if(holder == (char)0x20) //This should jump over any whitespace
                {
                 continue;
                }
                else
                {
                   currentItemName[currentItemNameSize] = holder;
                   currentItemNameSize++;
                }
            }
            //do we need to put a dot insert here?
            for(j = 0; j < 3; j++) //this loop gets the extention
            {
                holder = directory[currentOffset + 8 + j];
            
                if(holder == (char)0x20)
                {
                    continue;
                }
                else
                {
                    currentItemName[currentItemNameSize] = holder;
                    currentItemNameSize++;
                }
            }
            
            if(currentItemName == itemName)
            {
                fileExists = TRUE;
            }
            else
            {
                currentOffset = currentOffset + 0x20;
            }
        }
        
    }
    
    if(fileExists == FALSE)
    {
        return -1;
    }
    else
    {
        return currentOffset; //this will pass back the postion the file is at, or
        //-1 if the file doesn't exist at all
    }
 
}