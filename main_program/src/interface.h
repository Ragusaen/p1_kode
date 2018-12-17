#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"


void cli_init(int argc, const char **argv);

int _cli_run_command(const char *command, const char **argv, Commands cmdset);


#endif