#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>

#include "features.h"
#include "test.h"


void print_command_list(void);
void print_trained_features(FeatureSet);
void print_evaluation(EvaluationSet);
void print_confusion_matrix(ConfusionMatrix);
void print_headline_features(uint8_t, int);


#endif