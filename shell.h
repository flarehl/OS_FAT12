#ifndef F_SUPPORT_H
#define F_SUPPORT_H

#define DEFAULT_BUF_SIZE 50

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "./utilities/shmem.h"


/* FUNCTIONS */

void executeCmd(char** args);

char* getInput();

char** parseInput(char*);


#endif
