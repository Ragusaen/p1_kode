#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdint.h>
#include <math.h>

#include "io/errorhandler.h"
#include "ext/double.h"
#include "headline.h"
#include "features.h"


void classifier_classify_dataset(DataSet, FeatureSet, double);
int8_t classifier_classify_headline(Headline *, FeatureSet, double);
void classifier_score_dataset(DataSet, FeatureSet);
double classifier_calculate_threshold(DataSet, FeatureSet);


#endif
