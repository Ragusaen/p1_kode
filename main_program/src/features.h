#ifndef FEATURES_H
#define FEATURES_H

#define FEATURE_COUNT 6
#define MIN_WORD_LENGTH 8
#define MAX_AVERAGE_WORD_LENGTH 6
#define MIN_TOTAL_LENGTH 30
#define AMOUNT_OF_SPECIAL_WORDS 7

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


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

char *str_lwr( char *str );

#endif
