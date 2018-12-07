#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include "headline.h"

double recall(struct Headline *data_in, int data_amount);
int precision(struct Headline *data_in, int data_amount);

#endif
