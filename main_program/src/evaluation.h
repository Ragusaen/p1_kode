#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include "headline.h"
#include "classifier.h"
#include "double.h"

#define ROC_POINTS 100

typedef struct ResultCounter {
    uint16_t P, N, TP, FP;
} ResultCounter;

typedef struct ConfusionMatrix {
    uint16_t total, P, N, TP, FP, FN, TN, PP, PN;
    double threshold, prior, TPR, TNR, PPV, NPV, FNR, FPR, FDR, FOR, ACC, LRP, LRN, DOR, F1, MCC, BM, MK;
} ConfusionMatrix;

typedef struct EvaluationSet {
    ConfusionMatrix *data;
    int count;
} EvaluationSet;

typedef struct ROC_point {
    double TPR, FPR, threshold;
} ROC_point;

typedef struct ROC_set {
    ROC_point* points;
    int count;
} ROC_set;

ConfusionMatrix evaluate_classification(Headline *headlines, int headline_count);
ResultCounter _count_true_false_positives(Headline *headlines, int headline_count);
ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP);

ROC_set calculate_ROC(Headline *headlines, int headline_count);
double calculate_AUC(ROC_set roc);
void write_ROC_file(ROC_set roc);

/* DEPRECATED
double calculate_ROC_AUC(Headline *headlines, int headline_count);
void _get_min_max_probs( Headline *headlines, int count, double *min, double *max );
*/

int _compare_probabilities(const void *pa, const void *pb);

#endif
