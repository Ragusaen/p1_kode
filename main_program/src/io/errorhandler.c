#include "io/errorhandler.h"

/**
 * Non terminating error with message
 */

void error(char *msg)
{
    fprintf(stderr, "Error: %s", msg);
    
    if (errno != 0)
        fprintf(stderr, " (%s)", strerror(errno));

    fprintf(stderr, "\n");
}


/**
 * Terminating error with message
 */

void fatal(char *msg)
{
    error(msg);
    exit(EXIT_FAILURE);
}


/**
 * Terminating error without message
 */

void fatal_error(void)
{
    perror("Error");
    exit(EXIT_FAILURE);
}