#ifndef TRAIN_H
#define TRAIN_H

#include <stdint.h>
#include <math.h>
#include "headline.h"
#include "features.h"

FeatureSet train_features(DataSet set);

void _add_feature_count(Headline headline, FeatureSet featureset);

#endif