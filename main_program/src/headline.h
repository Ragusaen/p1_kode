#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HEADLINE_H
#define HEADLINE_H

#define FILE_BUFFER_LENGTH 256

#include "features.h"

/* Struct for headlines for further processing */
typedef struct Headline {
    char    *content;
    uint8_t labeled_clickbait;
    uint8_t classified_clickbait;
    uint8_t feature_vector;
    double  prob_cb;
} Headline;

typedef struct DataSet {
    Headline *data;
    int count;
} DataSet;

DataSet import_headline_csv(char file_path[]);

int _count_headlines(FILE *fp);
void _read_headlines(FILE *fp, DataSet dataset);

#endif
