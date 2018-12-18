#ifndef FEATURES_H
#define FEATURES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "io/errorhandler.h"

#define FEATURE_NAME_LEN 28

typedef struct Feature {
    double   prob_cb_given_feature;
    double   prob_feature;
    uint16_t feature_count;
    uint16_t feature_cb_count;
    uint8_t  (*has_feature)(char*);
    char name[FEATURE_NAME_LEN];
} Feature;

typedef struct FeatureSet { Feature *features; int count; } FeatureSet;


FeatureSet features_import(void);


#endif
