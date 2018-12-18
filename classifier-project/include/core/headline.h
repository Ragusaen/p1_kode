#ifndef HEADLINE_H
#define HEADLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/errorhandler.h"
#include "features.h"


typedef struct Headline {
    char    *content;
    uint8_t labeled_clickbait;
    uint8_t classified_clickbait;
    uint8_t feature_vector;
    double  prob_score;
} Headline;

typedef struct DataSet { Headline *data; int count; } DataSet;


int headline_import_dataset(char *, DataSet *);


#endif
