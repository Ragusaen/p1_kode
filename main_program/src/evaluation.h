#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include "headline.h"
#include "classifier.h"

#define ROC_POINTS 100
#define ROC_MARGIN 0.01

typedef struct ROC_point {
    double tpr;
    double fpr;
    double threshold;
} ROC_point;

typedef struct TFPNCounter {
    uint16_t total, P, N, TP, FP, FN, TN, PP, PN;
} TFPNCounter;

typedef struct ConfusionMatrix {
    TFPNCounter c;
    double prior, TPR, TNR, PPV, NPV, FNR, FPR, FDR, FOR, ACC, LRP, LRN, DOR, F1, MCC, BM, MK;
} ConfusionMatrix;

double recall(Headline *data_in, int data_amount);
double precision(Headline *data_in, int data_amount);

double calculate_ROC_AUC(Headline *headlines, int headline_count);

TFPNCounter count_results(Headline *data_in, int data_amount);
ConfusionMatrix calc_confusion_matrix(Headline *data_in, int data_amount);

void _get_min_max_probs( Headline *headlines, int count, double *min, double *max );

#endif
