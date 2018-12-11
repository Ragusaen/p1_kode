#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include "headline.h"
#include "classifier.h"

#define ROC_POINTS 100

typedef struct ROC_point {
    double tpr;
    double fpr;
    double threshold;
} ROC_point;

double recall(Headline *data_in, int data_amount);
double precision(Headline *data_in, int data_amount);

ROC_point *calculate_ROC(Headline *headlines, int count);

#endif
