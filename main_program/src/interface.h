#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorhandler.h"
#include "commands.h"


#define CLI_BUFFER_SIZE 1024
#define CLI_ARG_SIZE 128
#define CLI_DELIMITER " \t\r\n"


void cli_init(int argc, const char **argv);

void _cli_loop(Commands cmdset);
int _cli_run_command(const char *command, const char **argv, Commands cmdset);
char** _parse_argv(char *line);


#endif