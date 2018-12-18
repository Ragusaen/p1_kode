#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/errorhandler.h"
#include "headline.h"
#include "classifier.h"


typedef struct ResultCounter { uint16_t P, N, TP, FP, thresholds; } ResultCounter;

typedef struct ConfusionMatrix {
    uint16_t total, P, N, TP, FP, FN, TN, PP, PN;
    double threshold, prior, TPR, TNR, PPV, NPV, FNR, FPR, FDR, FOR, ACC, LRP, LRN, DOR, F1, MCC, BM, MK;
} ConfusionMatrix;

typedef struct EvaluationSet { ConfusionMatrix *data; int count; } EvaluationSet;


EvaluationSet test_classifier(DataSet dataset);
ConfusionMatrix test_classification(DataSet dataset, double threshold);
double test_calculate_auc(EvaluationSet evaluation);
void test_export_file(EvaluationSet evaluation, const char *filename);


#endif
