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

typedef struct ResultCounter {
    double P, N, TP, FP, TN, FN, TPR, TNR, PPV, NPV, FNR, FPR, FDR, FOR, ACC, F1, MCC, BM, MK;
} ResultCounter;

double recall(Headline *data_in, int data_amount);
double precision(Headline *data_in, int data_amount);

ROC_point *calculate_ROC(Headline *headlines, int count);

void print_confusion_matrix(Headline *data_in, int data_amount);
ResultCounter _count_results(Headline *data_in, int data_amount);

#endif
