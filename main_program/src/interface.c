#include "interface.h"

/**
 * Initializes the command line interface.
 * 
 * @param argc  number of arguments
 * @param argv  array of arguments
 */

void cli_init(int argc, const char **argv)
{
    Commands cmdset = import_commands();

    /* only one argument (the executed path) */
    if (argc == 1)
        /* continuous shell-like loop */
        _cli_loop(cmdset);
    else
        /* run the command given as the first argument */
        _cli_run_command(argv[1], argv, cmdset);
}


/**
 * Runs a shell-like loop, taking commands until 'exit' is entered or control+c pressed.
 */

void _cli_loop(Commands cmdset)
{
    int running;
    char line[CLI_BUFFER_SIZE], **argv;

    /* print initial information */
    printf("Enter 'help' to get a list of commands. Enter 'exit' to terminate program.\n\n");

    do {
        /* get user input */
        printf("> ");
        gets(line);

        /* parse the input */
        argv = _parse_argv(line);

        /* run command parsed */
        running = _cli_run_command(argv[1], (const char**)argv, cmdset);
    }
    while(running);
}


/**
 * Run a command.
 */

int _cli_run_command(const char *command, const char **argv, Commands cmdset)
{
    int i;

    /* if empty continue loop (if in _cli_loop) */
    if (strlen(command) == 0) return 1;

    /* search for command */
    for (i = 0; i < cmdset.count; i++) {
        if (strcmp(command, cmdset.commands[i].name) == 0) {
            /* pass the commands return value */
            return (*cmdset.commands[i].func)(argv);
        }
    }

    /* no match */
    error("Unknown command");
    return 0;
}


/**
 * Parse a string of arguments to an array.
 */

char** _parse_argv(char *line)
{
    int buffer_size = CLI_ARG_SIZE, i = 1;
    char **argv, *token;

    if ((argv = malloc(buffer_size * sizeof(char*))) == NULL) fatal_error();

    /* set first item to nothing (to match argv format of main) */
    argv[0] = NULL;

    token = strtok(line, CLI_DELIMITER);

    while (token != NULL) {
        argv[i++] = token;

        if (i >= buffer_size) {
            /* dynamically increase argv size */
            buffer_size += CLI_ARG_SIZE;
            if ((argv = realloc(argv, buffer_size * sizeof(char*))) == NULL) fatal_error();
        }

        token = strtok(NULL, CLI_DELIMITER);
    }

    /* end the array */
    argv[i] = NULL;

    return argv;
}