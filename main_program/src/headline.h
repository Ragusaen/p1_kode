#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HEADLINE_H
#define HEADLINE_H

#define FILE_BUFFER_LENGTH 256

#include "features.h"

/* Struct for headlines for further processing */
typedef struct Headline {
  char    *title;
  uint8_t labeled_clickbait;
  uint8_t classified_clickbait;
  uint8_t feature_vector;
  double  prob_cb;
} Headline;

int import_csv(Headline **headlines, int *headline_count, char *file_path);

FILE *_open_file();
int _count_headlines(FILE *dataset);
void _read_headlines(FILE *dataset, Headline headlines[], int headline_count );

#endif
