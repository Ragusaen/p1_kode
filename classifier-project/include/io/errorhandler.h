#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(char *);
void fatal(char *);
void fatal_error(void);

#endif