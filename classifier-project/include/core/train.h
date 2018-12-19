#ifndef TRAIN_H
#define TRAIN_H

#include <stdint.h>
#include <math.h>

#include "headline.h"
#include "features.h"


int train_load_features(FeatureSet *);
FeatureSet train_features(DataSet);


#endif