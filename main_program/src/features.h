#ifndef FEATURES_H
#define FEATURES_H

#define FEATURE_COUNT 5
#define MAX_WORD_LENGTH 8
#define MAX_AVERAGE_WORD_LENGTH 6
#define MAX_TOTAL_LENGTH 30 
#define AMOUNT_OF_SPECIAL_WORDS 5

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
double average_length(char str_in[]);
int special_words(char str_in[]);
int total_length(char str_in[]);

#endif
