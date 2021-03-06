		#!/bin/bash
#
# Name: Fat 12 File System
#
# Purpose:
#    To test the integrity, funcionality, and stability of a FAT12 file system
# 	 Through pushing the limits on all its functions and intentionally trying to crash it
#
# Notes:
# 1)
#
#    
#
# Return codes:
#  0 = All commands were successful
#  1 = At least one command failed, see the output file 
#      and search for the keyword "ERROR".
#
########################################################


#coproc ./fat12

{ coproc ./fat12 { ./fat12 logfile ;} >&3 ;} 3>&1

#================
# Test Functions
#================
mypbs()
{
	echo "pbs" >&${COPROC[1]}
}

mypfe()
{
	echo "pfe" >&${COPROC[1]}
}

mydf()
{
	echo "df" >&${COPROC[1]}
}

mycat() #x - filename
{
	echo "cat" += $1 >&${COPROC[1]}
}

mycd() #x - directory
{
	echo "cd" += $1 >&${COPROC[1]}
}

myls() #x - directory
{
	echo "ls" += $1 >&${COPROC[1]}
}

mymkdir() #x - directory
{
	echo "mkdir" += $1 >&${COPROC[1]}
}

mypwd()
{
	echo "pwd" += $1 >&${COPROC[1]}
}

myrm() #x - directory
{
	echo "rm" += $1 >&${COPROC[1]}
}

myrmdir() #x - directory
{
	echo "rmdir" += $1 >&${COPROC[1]}
}

mytouch() #x - directory
{
	echo "touch" += $1 >&${COPROC[1]}
}

terminate()
{
 echo "The execution of $CALLER was not successful."
 echo "$CALLER terminated, exiting now with rc=1."
 dateTest=`date`
 echo "End of testing at: $dateTest"
 echo ""
 exit 1
}

#================
# Functions End
#================

CALLER=`basename $0`         # The Caller name
SILENT="no"                  # User wants prompts
let "errorCounter = 0"
#SHELL=$1

# start first process as a coprocess to the current shell

# now ${COPROC[0]} contains the number of an open (input) file descriptor
# connected to the output of proc1, and ${COPROC[1]} the number of an
# open (output) file descriptor connected to the input of proc1.

echo "Hello World" >&${COPROC[1]}

#/shell

#/my/bash/script < echo 'This string will be sent to stdin.'

#==================
# TEST BEGIN
#==================

#call each commande with all possible input in order and print out status reports
# Test - PBS
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mypbs

# Test - PFE
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mypfe

# Test - DF
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mydf

# Test - TOUCH
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mytouch

mytouch "testfile.txt"

mytouch "lsdfja023r.!!@#$"

mytouch "file file2"

# Test - CAT
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mycat 

mycat "120498=123-1.234."

mycat "testfile.txt"

mycat "testfile.txt faketest.txt"

# Test - CD
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mycd

mycd "../"

mycd "../garbage"

mycd "../ /home"

#reset directory
mycd "../"

# Test - LS
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

myls 

myls "testfile.txt"

myls "jjafdji.////."

myls "testfile.txt testfile.txt"

# Test - MKDIR
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

mymkdir

mymkdir "testdir"

mymkdir "../020-1=!@"

mymkdir "garbage1 garbage2"

# Test - PWD
# 
# 2 Cases
# 1) no arguments
# 2) one argument - garbage

mypwd

mypwd "asfdf "


# Test - RM
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

myrm

myrm "testfile"

myrm "testfile"

myrm "file1 file2"

# Test - RMDIR
# 
# 4 Cases
# 1) no arguments
# 2) one argument - not garbage
# 3) one argument - garbage
# 4) many arguments

myrmdir

myrmdir "test"

myrmdir "test"

myrmdir "dir dir2"

#==================
# TEST END
#==================


