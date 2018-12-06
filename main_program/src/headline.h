#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HEADLINE_H
#define HEADLINE_H

#define MAX_HEADLINE_LENGTH 256

#include "features.h"

/* Struct for headlines for further processing */
typedef struct Headline {
  char title[MAX_HEADLINE_LENGTH];
  int labeled_clickbait : 1;
  int classified_clickbait : 1;
  int feature_vector : FEATURE_COUNT;
} Headline;

FILE *open_file();
int count_headlines(FILE *dataset);
void read_headlines(FILE *dataset, Headline headlines[]);

#endif
