#ifndef FEATURES_H
#define FEATURES_H

#define FEATURE_COUNT 7

#include <stdint.h>
#include <string.h>

typedef struct Feature {
    double   prob_cb_given_feature;
    double   prob_feature;
    uint16_t feature_count;
    uint16_t feature_cb_count;
    uint8_t  (*has_feature)(char*);
} Feature;

uint8_t longest_word(char str_in[]);
uint8_t average_length(char str_in[]);
uint8_t special_words(char str_in[]);
uint8_t total_length(char str_in[]);
uint8_t has_cb_punctuation(char str_in[]);
uint8_t has_cb_hastag(char str_in[]);
uint8_t begins_with_number(char str_in[]);

#endif
