#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "arcfour.h"
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/random.h>


//int8 *readkey(char *);
int8 *securerand(int16);
int main(int, char**);