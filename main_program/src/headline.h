#ifndef HEADLINE_H
#define HEADLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorhandler.h"
#include "features.h"

#define FILE_BUFFER_LENGTH 256


/* Struct for headlines for further processing */
typedef struct Headline {
    char    *content;
    uint8_t labeled_clickbait;
    uint8_t classified_clickbait;
    uint8_t feature_vector;
    double  prob_score;
} Headline;

typedef struct DataSet {
    Headline *data;
    int count;
} DataSet;


DataSet import_headline_csv(char file_path[]);

int _count_headlines(FILE *fp);
void _read_headlines(FILE *fp, DataSet dataset);
void _parse_headline(Headline *headline, char *buffer, char *token);


#endif
