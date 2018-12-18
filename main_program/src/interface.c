#include "interface.h"

#define CLI_BUFFER_SIZE 1024
#define CLI_ARG_SIZE 64
#define CLI_DELIMITER " \t\r\n"

/* internal functions */
static void _run_shell_loop(Commands);
static int _run_command(const char *, const char **, Commands);
static char** _parse_argv(char *);
static int _read_string(char *, char *, char *);


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
    int argc = 1, i = 0, len, buffer_size, is_str = 0;
    char **argv;

    len = (int) strlen(line);
    buffer_size = CLI_ARG_SIZE;

    if ((argv = (char**) malloc(buffer_size * sizeof(char*))) == NULL) fatal_error();

    while (i < len) {
        if (argc >= buffer_size) {
            /* dynamically increase argv size */
            buffer_size += CLI_ARG_SIZE;
            if ((argv = (char**) realloc(argv, buffer_size * sizeof(char*))) == NULL) fatal_error();
        }

        is_str = line[i] == '"';

        if (is_str || strchr(CLI_DELIMITER, line[i]) == NULL) {
            if (is_str) i++;

            if ((argv[argc] = (char*) malloc(CLI_BUFFER_SIZE)) == NULL) fatal_error();

            i += _read_string(argv[argc], line + i, is_str ? "\"" : CLI_DELIMITER);
            
            argc++;
        }
        else i++;
    }

    /* end the array */
    argv[argc] = NULL;

    return argv;
}


/**
 * Read a string until quotation mark met
 */

int _read_string(char *dest, char *src, char *delimiters)
{
    int i = 0, len = (int) strlen(src);
    char buffer[CLI_BUFFER_SIZE];

    while (i < len && strchr(delimiters, src[i]) == NULL) {
        buffer[i] = src[i];
        ++i;
    }

    buffer[i] = '\0';
    strcpy(dest, buffer);

    return (i + 1);
}
