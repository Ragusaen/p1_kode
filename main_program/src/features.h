#ifndef FUNCTIONS_STRINGS_H
#define FUNCTIONS_STRINGS_H

#define FEATURE_COUNT 5

#include <stdint.h>

typedef struct Feature {
    double   prob_cb_given_feature;
    double   prob_feature;
    uint16_t feature_count;
    uint16_t feature_cb_count;
    uint8_t  (*has_feature)(char*);
} Feature;

uint8_t laengste_ord(char str_in[]);
double gaennemsnit_laengde(char str_in[]);
int bestemte_ord(char str_in[]);
int samlet_laengde(char str_in[]);
int ord_laengde(char str_in[]);

#endif
