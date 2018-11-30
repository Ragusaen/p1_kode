#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADLINE_LENGTH 256

/* Struct til headlinesne, der skal bearbejdes. */
typedef struct Headlines {
  char headline[MAX_HEADLINE_LENGTH];
  int clickbait_or_not;
} Headlines;

FILE *open_file();
int count_headlines(FILE *dataset);
void read_headlines(FILE *dataset, Headlines headlines[]);
