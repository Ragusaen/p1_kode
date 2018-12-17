#include "interface.h"

void cli_init(int argc, const char **argv)
{
    Commands cmdset = import_commands();

    if (argc > 1)
        _cli_run_command(argv[1], argv, cmdset);
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