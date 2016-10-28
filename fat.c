/******************************************************************************
 * main: Sample for starting the FAT project.
 *
 * Authors:  Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *           March, 2004.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "fatSupport.h"

/******************************************************************************
 * You must set these global variables:
 *    FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                      filesystem)	
 *    BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 *
 * You may use these support functions (defined in FatSupport.c)
 *    read_sector
 *    write_sector
 *    get_fat_entry
 *    set_fat_entry
 *****************************************************************************/

int main(int argc, char **argv)
{
	char** args;
	char* line;
	
	while( TRUE ) {

		line = getInput();
		args = parseInput(line);

		if( (strncmp(args[0], "exit", 4) == 0) || (strncmp(args[0], "logout", 6) == 0) )
			break;

		executeCmd(args);

	} 

	return 0;
}

