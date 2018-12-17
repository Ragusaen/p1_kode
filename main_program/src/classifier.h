#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdint.h>
#include <math.h>

#include "errorhandler.h"
#include "headline.h"
#include "features.h"
#include "double.h"

void classify_dataset(DataSet set, FeatureSet featureset, double threshold);
int8_t classify(Headline *headline, FeatureSet featureset, double threshold);
void score_dataset(DataSet set, FeatureSet featureset);
double calculate_threshold(DataSet set, FeatureSet featureset);

void _score_headline(Headline *headline, FeatureSet featureset);
uint8_t _get_feature_vector(char str_in[], FeatureSet featureset);
double _calculate_cb_prob(uint8_t feature_vector, FeatureSet featuresset);
double _prob_given_not_feature(double pcbf, double pf);

#endif
