#include "interface.h"

#define CLI_BUFFER_SIZE 1024
#define CLI_ARG_SIZE 128
#define CLI_DELIMITER " \t\r\n"

/* internal functions */
static void _run_shell_loop(Commands);
static int _run_command(const char *, const char **, Commands);
static char** _parse_argv(char *);


/**
 * Initializes the command line interface.
 *
 * @param argc  number of arguments
 * @param argv  array of arguments
 */

void interface_init(int argc, const char **argv)
{
    Commands cmdset = commands_import();

    /* only one argument (the executed path) */
    if (argc == 1)
        /* continuous shell-like loop */
        _run_shell_loop(cmdset);
    else
        /* run the command given as the first argument */
        _run_command(argv[1], argv, cmdset);
}


/**
 * Runs a shell-like loop, taking commands until 'exit' is entered or control+c pressed.
 */

void _run_shell_loop(Commands cmdset)
{
    int running;
    char line[CLI_BUFFER_SIZE], **argv;

    /* print initial information */
    printf("Enter 'help' to get a list of commands. Enter 'exit' to terminate program.\n\n");

    do {
        char c;

        /* get user input */
        printf("> ");
        scanf("%[^\n]s",line);

        /* Clear input buffer */
        while( (c = getchar()) != '\n' && c != EOF );

        /* parse the input */
        argv = _parse_argv(line);

        /* run command parsed */
        running = _run_command(argv[1], (const char**)argv, cmdset);
    }
    while(running);
}


/**
 * Run a command.
 */

int _run_command(const char *command, const char **argv, Commands cmdset)
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
    int buffer_size = CLI_ARG_SIZE, i = 1, in_string = 0;
    char **argv, *token, temp[CLI_BUFFER_SIZE];

    if ((argv = malloc(buffer_size * sizeof(char*))) == NULL) fatal_error();

    /* set first item to nothing (to match argv format of main) */
    argv[0] = NULL;

    token = strtok(line, CLI_DELIMITER);

    while (token != NULL) {
        if (in_string) {
            memset(temp, '\0', CLI_BUFFER_SIZE);
            temp[0] = ' ';
            strncpy(temp + 1, token, strlen(token));

            if (temp[strlen(temp)-1] == '"') {
                in_string = 0;
                temp[strlen(temp)-1] = '\0';
            }

            strcat(argv[i-1], temp);
        }
        else {
            argv[i++] = (in_string = token[0] == '"') ? token + 1 : token;
        }

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
