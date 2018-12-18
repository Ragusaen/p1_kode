#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errorhandler.h"

char* load_config(const char *);
int save_config(const char *, const char *);


#endif