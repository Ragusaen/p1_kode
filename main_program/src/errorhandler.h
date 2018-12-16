#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(char *msg);
void fatal(char *msg);
void fatal_error(void);

#endif