#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define PROB_THRESHOLD 0.003

#include <stdint.h>
#include <math.h>

#include "headline.h"
#include "features.h"
#include "double_array.h"

int8_t classify( Headline *headline, Feature *features, double threshold );
void classify_array( Headline *headlines, uint16_t headline_count, Feature *features, double threshold );
Feature* calculate_feature_array( Headline* headlines, uint16_t headline_count );
double calculate_threshold(Headline* headlines, uint16_t headline_count, Feature *features);

void _set_feature_vector( Headline *headline, Feature *features );
void _add_feature_count( Headline headline, Feature *features );
double _calculate_cb_prob( Headline headline, Feature *features );
double _bernoulli_nb( Headline headline, Feature *features );

#endif
