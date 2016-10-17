/******************************************************************************
 * main: Sample for starting the FAT project.
 *
 * Authors:  Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *           March, 2004.
 *****************************************************************************/

#include <stdio.h>
#include "fatSupport.h"
#include "bootSector.h"

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

FILE* FILE_SYSTEM_ID; //just declare, cant initialize outside main
int BYTES_PER_SECTOR = 512; //correct number set
BootSector BOOT_SECTOR_ATTRIBUTES;

extern void pfe(int, int);
extern void pbs();

/******************************************************************************
 * main: an example of reading an item in the boot sector
 *****************************************************************************/

int main(int argc, char **argv)
{
	/* shell goes here */
	//char* commands = (char*)malloc(sizeof(argv[0]) * sizeof(char*));
	//commands = argv[0];

	//while ( argv[0] != "exit" || argv[0] != "EXIT") {
		//print root with arrow: root>

		switch ( atoi(argv[1]) ) {
		case 'pbs':
			pbs();
			break;

		case 'pfe':
			pfe( argv[1], argv[2] );
			break;

		case 'pwd':
			//nothing yet
			break;

		default:
			break;
		}
	//}

		system("pause");
	return 0;
}
