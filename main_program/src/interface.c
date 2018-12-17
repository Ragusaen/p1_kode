#include "interface.h"

void cli_init(int argc, const char **argv)
{
    Commands cmdset = import_commands();

    if (argc == 1)
        _cli_loop(cmdset);
    else
        _cli_run_command(argv[1], argv, cmdset);
}

void _cli_loop(Commands cmdset)
{
    int running;
    char line[CLI_BUFFER_SIZE], **argv;

    printf("Enter 'help' to get a list of commands. Enter 'exit' to terminate program.\n\n");

    do {
        printf("> ");
        gets(line);

        argv = _parse_argv(line);

        running = _cli_run_command(argv[1], (const char**)argv, cmdset);
    }
    while(running);
}

int _cli_run_command(const char *command, const char **argv, Commands cmdset)
{
    int i;

    for (i = 0; i < cmdset.count; i++) {
        if (strcmp(command, cmdset.commands[i].name) == 0) {
            return (*cmdset.commands[i].func)(argv);
        }
    }

    error("Unknown command");
    return 0;
}

char** _parse_argv(char *line)
{
    int buffer_size = CLI_ARG_SIZE, i = 0;
    char **argv, *token;

    if ((argv = malloc(buffer_size * sizeof(char*))) == NULL) fatal_error();

    argv[i++] = NULL;

    token = strtok(line, CLI_DELIMITER);

    while (token != NULL) {
        argv[i++] = token;

        if (i >= buffer_size) {
            buffer_size += CLI_ARG_SIZE;
            if ((argv = realloc(argv, buffer_size * sizeof(char*))) == NULL) fatal_error();
        }

        token = strtok(NULL, CLI_DELIMITER);
    }

    argv[i] = NULL;

    return argv;
}