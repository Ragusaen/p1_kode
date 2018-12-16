#include "errorhandler.h"

void error(char *msg)
{
    fprintf(stderr, "Error: %s", msg);
    
    if (errno != 0)
        fprintf(stderr, " (%s)", strerror(errno));

    fprintf(stderr, "\n");
}

void fatal(char *msg)
{
    error(msg);
    exit(EXIT_FAILURE);
}

void fatal_error(void)
{
    perror("Error");
    exit(EXIT_FAILURE);
}