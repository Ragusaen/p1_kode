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

int import_csv(Headline **headlines, int *headline_count, char *file_path);

FILE *_open_file();
int _count_headlines(FILE *dataset);
void _read_headlines(FILE *dataset, Headline headlines[]);

#endif
