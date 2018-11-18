/*
    Automatic detection of clickbait


*/

/* This controls wether or not the program is in debugging mode for conditional compilation */
#define DEBUG_MODE 1

/* Include libraries */
#include <stdlib.h>

/* Include headers */
#include "datapoint.h"

/* Entrypoint for the program */
int main( int argc, const char* argv[] ) {

    /* This is only example code */
    Datapoint dp = {"Hello World!", 0.0};

    dp.clickbait_score += 10;

    return EXIT_SUCCESS;
}
