#ifndef __SHMEM_H__
#define __SHMEM_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "support.h"
#include "../structures/cwd.h"

#define SHMEMSIZE sizeof(CurrentPath)
#define SHMEMKEY 5689

CurrentPath CPATH;

bool accessShmem(void **ptr);
bool createShmem(void **ptr); //double pointer because argument passed in is reference to pointer
bool detachShmem(void *ptr);

#endif