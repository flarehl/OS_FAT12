#!/bin/bash

#cp backupFloppies/floppy1 floppies/floppy1
#cp backupFloppies/floppy2 floppies/floppy2
#cp backupFloppies/floppy3 floppies/floppy3

cp backupFloppies/floppy1 floppies/floppy1
cp backupFloppies/floppy2 floppies/floppy2
cp backupFloppies/floppy3 floppies/floppy3
make
./fat12
