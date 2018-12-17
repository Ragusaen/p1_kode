#ifndef TRAIN_H
#define TRAIN_H

#include <stdint.h>
#include <math.h>

#include "headline.h"
#include "features.h"

#define TRAINED_FEATURES_FN "trainedfeatures.bin"

int load_trained_features(FeatureSet *featureset);
FeatureSet train_features(DataSet dataset);

void _add_feature_count(Headline headline, FeatureSet featureset);
void _calculate_feature_probabilities(Feature *feature, int data_count);

int _save_trained_features(FeatureSet featureset);
void _copy_feature(Feature *target, Feature original);

#endif