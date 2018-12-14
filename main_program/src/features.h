#ifndef FEATURES_H
#define FEATURES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define FEATURE_COUNT 11
#define FEATURE_NAME_LEN 28

#define MIN_WORD_LENGTH 8
#define MIN_AVERAGE_WORD_LENGTH 4.5
#define MIN_TOTAL_LENGTH 40
#define AMOUNT_OF_SPECIAL_WORDS 7
#define AMOUNT_OF_PRONOUNS 10
#define MAX_STOP_WORDS 2
#define AMOUNT_OF_STOP_WORDS 13
#define MIN_CAPS_LEN 4


typedef struct Feature {
    double   prob_cb_given_feature;
    double   prob_feature;
    uint16_t feature_count;
    uint16_t feature_cb_count;
    uint8_t  (*has_feature)(char*);
    char name[FEATURE_NAME_LEN];
} Feature;

Feature* get_features();
void _add_feature(char str[], uint8_t (*func)(char*), uint8_t *i, Feature *features);

uint8_t has_no_long_word(char str_in[]);
uint8_t has_low_average_word_length(char str_in[]);
uint8_t is_short(char str_in[]);
uint8_t has_special_punctuation(char str_in[]);
uint8_t has_colon(char str_in[]);
uint8_t has_special_words(char str_in[]);
uint8_t has_pronouns(char str_in[]);
uint8_t has_stop_words(char str_in[]);
uint8_t has_adverbs(char str_in[]);
uint8_t has_no_numbers(char str_in[]);
uint8_t has_caps(char str_in[]);

char * _string_lower( char *str );
int _str_count_words( char *str, char **words, int word_count );
uint8_t _match_whole_word( char *str, char *word );
uint8_t _match_end_of_word( char *str, char *word );
uint8_t _match_word_condition(char str[], char word[], int (*comp_before)(int), int (*comp_after)(int));
int _char_is_punct_or_space(int c);

#endif
