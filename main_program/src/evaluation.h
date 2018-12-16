#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorhandler.h"
#include "headline.h"
#include "classifier.h"


typedef struct ResultCounter {
    uint16_t P, N, TP, FP, thresholds;
} ResultCounter;

typedef struct ConfusionMatrix {
    uint16_t total, P, N, TP, FP, FN, TN, PP, PN;
    double threshold, prior, TPR, TNR, PPV, NPV, FNR, FPR, FDR, FOR, ACC, LRP, LRN, DOR, F1, MCC, BM, MK;
} ConfusionMatrix;

typedef struct EvaluationSet {
    ConfusionMatrix *data;
    int count;
} EvaluationSet;


EvaluationSet evaluate_classifier(DataSet dataset);
ConfusionMatrix evaluate_classification(DataSet dataset, double threshold);

double calculate_AUC(EvaluationSet set);

void write_evaluation_file(EvaluationSet set, char *filename);

ResultCounter _count_thresholds_positives_negatives(DataSet dataset);
ResultCounter _count_true_false_positives(DataSet dataset);
ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP, double threshold);
int _sort_by_probability_desc(const void *pa, const void *pb);

void _write_evaluation_data(FILE *fp, ConfusionMatrix data);
char* _csv_double(double n);

#endif
