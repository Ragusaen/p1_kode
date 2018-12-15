#ifndef RECALL_PRECISION_H
#define RECALL_PRECISION_H

#include <stdio.h>
#include <stdlib.h>
#include "headline.h"
#include "classifier.h"
#include "export_csv.h"


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


EvaluationSet evaluate_classifier(Headline *headlines, int headline_count);
ConfusionMatrix evaluate_classification(Headline *headlines, int headline_count, double threshold);

double calculate_AUC(EvaluationSet set);

void write_evaluation_file(EvaluationSet set, char *filename);

ResultCounter _count_true_false_positives(Headline *headlines, int headline_count);
ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP, double threshold);
int _compare_probabilities(const void *pa, const void *pb);

/* DEPRECATED
double calculate_ROC_AUC(Headline *headlines, int headline_count);
void _get_min_max_probs( Headline *headlines, int count, double *min, double *max );
*/

#endif
