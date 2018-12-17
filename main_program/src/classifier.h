#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdint.h>
#include <math.h>

#include "errorhandler.h"
#include "headline.h"
#include "features.h"
#include "double.h"

void classify_dataset(DataSet dataset, FeatureSet featureset, double threshold);
int8_t classify(Headline *headline, FeatureSet featureset, double threshold);
void score_dataset(DataSet dataset, FeatureSet featureset);
double calculate_threshold(DataSet dataset, FeatureSet featureset);

void _score_headline(Headline *headline, FeatureSet featureset);
uint8_t _get_feature_vector(char str_in[], FeatureSet featureset);
double _calculate_prob_score(uint8_t feature_vector, FeatureSet featuresset);
double _prob_given_not_feature(double pcbf, double pf);

#endif
