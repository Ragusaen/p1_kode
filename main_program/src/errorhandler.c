#include "errorhandler.h"

void fatal(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void fatal_error(void)
{
    fatal("Error");
}