#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define PROB_THRESHOLD 0.5

#include <stdint.h>

#include "headline.h"
#include "features.h"

int classify( Headline *headline, Feature *features );
void classify_array( Headline *headlines, uint16_t headline_count, Feature *features );
Feature* calculate_feature_array( Headline* headlines, uint16_t headline_count );

void _set_feature_vector( Headline *headline, Feature *features );
void _add_feature_count( Headline headline, Feature *features );
double _calculate_cb_prob( Headline headline, Feature* features );

#endif