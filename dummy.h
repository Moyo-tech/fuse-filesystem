#ifndef DUMMY_H
#define DUMMY_H

#define FUSE_USE_VERSION 30
#define block_size 1024

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h> 
#include "dummy.c"
#endif