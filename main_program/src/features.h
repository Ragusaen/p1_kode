#ifndef FEATURES_H
#define FEATURES_H

#define FEATURE_COUNT 8
#define MIN_WORD_LENGTH 8
#define MIN_AVERAGE_WORD_LENGTH 4.5
#define MIN_TOTAL_LENGTH 40
#define AMOUNT_OF_SPECIAL_WORDS 7
#define AMOUNT_OF_PRONOUNS 10
#define MAX_STOP_WORDS 2
#define AMOUNT_OF_STOP_WORDS 13

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

uint8_t has_long_word(char str_in[]);
uint8_t has_special_words(char str_in[]);
uint8_t is_long(char str_in[]);
uint8_t has_punctuation(char str_in[]);
uint8_t has_pronouns(char str_in[]);
uint8_t has_number(char str_in[]);
uint8_t has_long_average_words(char str_in[]);
uint8_t has_stop_words(char str_in[]);

char * _string_lower( char *str );
uint8_t _match_whole_word( char *str, char *word );
uint8_t _str_has_chars( char *str, char *chars );
uint8_t _str_count_words( char *str, char **words, int word_count );

#endif
