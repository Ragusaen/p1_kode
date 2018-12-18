#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdint.h>
#include <math.h>

#include "errorhandler.h"
#include "headline.h"
#include "features.h"
#include "double.h"


void classifier_classify_dataset(DataSet, FeatureSet, double);
int8_t classifier_classify_headline(Headline *, FeatureSet, double);
void classifier_score_dataset(DataSet, FeatureSet);
double classifier_calculate_threshold(DataSet, FeatureSet);


#endif
