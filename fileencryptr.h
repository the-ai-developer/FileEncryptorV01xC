#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "arcfour.h"
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/random.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>


void toggleEcho(bool);
int8 *securerand(int16);
int main(int, char**);