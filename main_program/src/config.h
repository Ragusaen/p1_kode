#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errorhandler.h"

#define CONFIG_FN "cbclassifier.config"
#define CONFIG_LINE_LEN 255
#define CONFIG_MAX_FILESIZE 1024

char* load_config(const char *key);
int save_config(const char *key, const char *value);

char* _get_config_key(char *line);
char* _get_config_value(char *line);


#endif