#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorhandler.h"
#include "config.h"
#include "print.h"
#include "classifier.h"
#include "headline.h"
#include "features.h"
#include "train.h"
#include "test.h"


#define COMMAND_NAME_LEN 10


typedef struct Command {
    char name[COMMAND_NAME_LEN];
    int (*func)(const char**);
} Command;

typedef struct Commands {
    Command *commands;
    int count;
} Commands;


Commands commands_import(void);


#endif