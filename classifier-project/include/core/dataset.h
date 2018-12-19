#ifndef HEADLINE_H
#define HEADLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "io/errorhandler.h"


typedef struct Headline {
    char    *content;
    uint8_t labeled_clickbait;
    uint8_t classified_clickbait;
    uint8_t feature_vector;
    double  prob_score;
} Headline;

typedef struct DataSet { Headline *data; int count; } DataSet;


int dataset_import(char *, DataSet *);


#endif
