#include "core/features.h"

#define MIN_WORD_LENGTH 8
#define MIN_AVERAGE_WORD_LENGTH 4.5
#define MIN_TOTAL_LENGTH 40
#define AMOUNT_OF_SPECIAL_WORDS 7
#define AMOUNT_OF_PRONOUNS 10
#define MAX_STOP_WORDS 2
#define AMOUNT_OF_STOP_WORDS 13
#define MIN_CAPS_LEN 4


/* internal functions */

static void _export_feature(char *, uint8_t (*)(char*), uint8_t, FeatureSet);

static uint8_t _feature_no_long_word(char *);
static uint8_t _feature_low_average_word_length(char *);
static uint8_t _feature_is_short(char *);
static uint8_t _feature_special_punctuation(char *);
static uint8_t _feature_special_words(char *);
static uint8_t _feature_pronouns(char *);
static uint8_t _feature_stop_words(char *);
static uint8_t _feature_adverbs(char *);
static uint8_t _feature_no_numbers(char *);
static uint8_t _feature_caps(char *);

static char * _string_lower(char *);
static int _str_count_words(char *, char **, int);
static uint8_t _match_whole_word(char *, char *);
static uint8_t _match_end_of_word(char *, char *);
static uint8_t _match_word_condition(char *, char *, int (*)(int), int (*)(int));
static int _char_is_punct_or_space(int);


/**
 * Exports the features
 */

FeatureSet features_import(void)
{
    uint8_t i = 0;
    FeatureSet exported;

    if ((exported.features = (Feature*) calloc(12, sizeof(Feature))) == NULL) fatal_error();

    _export_feature("no_long_word",            _feature_no_long_word,             i++, exported);
    _export_feature("low_average_word_length", _feature_low_average_word_length,  i++, exported);
    _export_feature("is_short",                _feature_is_short,                 i++, exported);
    _export_feature("special_punctuation",     _feature_special_punctuation,      i++, exported);
    _export_feature("special_words",           _feature_special_words,            i++, exported);
    _export_feature("pronouns",                _feature_pronouns,                 i++, exported);
    _export_feature("stop_words",              _feature_stop_words,               i++, exported);
    _export_feature("adverbs",                 _feature_adverbs,                  i++, exported);
    _export_feature("no_numbers",              _feature_no_numbers,               i++, exported);
    _export_feature("caps",                    _feature_caps,                     i++, exported);
    
    exported.count = i;

    return exported;
}

void _export_feature(char name[], uint8_t (*func)(char*), uint8_t i, FeatureSet exported)
{
    exported.features[i].has_feature = func;
    strncpy(exported.features[i].name, name, FEATURE_NAME_LEN);
}


/**
 * Checks if headline only contains words less than 8 characters long.
 */

uint8_t _feature_no_long_word(char str_in[])
{
    int longest_word_length = 0,
        curr_word_length = 0,
        i;

    for (i = 0; i <= strlen(str_in); i++) {
        /* if end-of-str or current char is punctuation or white-space */
        if (i == strlen(str_in) || ispunct(str_in[i]) || isspace(str_in[i])) {
            /* set longest_word_length if current word-length is larger */
            if (curr_word_length > longest_word_length)
                longest_word_length = curr_word_length;

            curr_word_length = 0;
        }
        else
            curr_word_length++;
    }

    return longest_word_length < MIN_WORD_LENGTH;
}


/**
 * Checks if the headline's average word length is below 4.5.
 */

uint8_t _feature_low_average_word_length(char str_in[])
{
    int i = 0,
        word_length_sum = 0,
        word_count = 0,
        word_length = 0;

    for (i = 0; i <= strlen(str_in); i++) {
        /* if end-of-str or current char is punctuation or white-space */
        if (i == strlen(str_in) || _char_is_punct_or_space(str_in[i])) {
            /* add word_length to sum, increase word_count, and reset word_length */
            if (word_length > 0) {
                word_length_sum += word_length;
                word_count++;
                word_length = 0;
            }
        }
        else
            word_length++;
    }

    return ((double) word_length_sum / (double) word_count) < MIN_AVERAGE_WORD_LENGTH;
}


/**
 * Checks if headline is less than 40 characters long.
 */

uint8_t _feature_is_short(char str_in[])
{
    return strlen(str_in) < MIN_TOTAL_LENGTH;
}


/**
 * Checks if headline contains special punctuation, ! ?
 */

uint8_t _feature_special_punctuation(char str_in[])
{
    return strpbrk(str_in, "!?") != NULL;
}


/**
 * Checks if headline contains special forward-referencing words.
 */

uint8_t _feature_special_words(char str_in[])
{
    char* words[AMOUNT_OF_SPECIAL_WORDS] = {
        "sådan", "derfor", "denne", "dette", "her", "så meget", "så lidt"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_SPECIAL_WORDS ) > 0;
}


/**
 * Checks if headline contains pronouns.
 */

uint8_t _feature_pronouns(char str_in[])
{
    char *words[AMOUNT_OF_PRONOUNS] = {
        "du", "han", "hun", "hende", "din", "jeg", "os", "de", "min", "dit"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_PRONOUNS ) > 0;
}


/**
 * Checks if headline contains more than 2 stop-words.
 */

uint8_t _feature_stop_words(char str_in[])
{
    char *words[AMOUNT_OF_STOP_WORDS] = {
        "og", "i", "at", "det", "er", "en", "på", "til", "med", "af", "ikke", "med", "til"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_STOP_WORDS ) > MAX_STOP_WORDS;
}


/**
 * Checks if headline contains adverbs ending in 'lig' or 'lige'.
 */

uint8_t _feature_adverbs(char str_in[])
{
    return _match_end_of_word( str_in, "lig") || _match_end_of_word( str_in, "lige");
}


/**
 * Checks if headline contains a number.
 */

uint8_t _feature_no_numbers(char str_in[])
{
	return strpbrk(str_in, "0123456789") == NULL;
}


/**
 * Checks if headline contains CAPS word longer than 3 characters
 */

uint8_t _feature_caps(char str_in[])
{
    int i, caps_length = 0, curr_length = 0;

    for (i = 0; i <= strlen(str_in); i++) {
        /* if end-of-str or current char is not uppercase */
        if (i == strlen(str_in) || !isalpha(str_in[i]) || str_in[i] != toupper(str_in[i])) {
            if (curr_length > 0) {
                if (caps_length < curr_length)
                    caps_length = curr_length;

                curr_length = 0;
            }
        }
        else
            curr_length++;
    }

    return caps_length >= MIN_CAPS_LEN;
}


/**
 * Transforms str to lowercase.
 */

char * _string_lower( char *str )
{
    int i;
    char* lwr = (char*) malloc( strlen(str) + 1);

    for ( i = 0; i < strlen(str) + 1; i++ ) {
        lwr[i] = tolower(str[i]);
    }

    return lwr;
}


/**
 * Counts matched whole words in str.
 */

int _str_count_words( char *str, char **words, int word_count )
{
    int i, matches = 0;
    char *lower_string = _string_lower(str);

    for ( i = 0; i < word_count; i++ ) {
        if ( _match_whole_word(lower_string, words[i]) )
            matches++;
    }

    free(lower_string);
    return matches;
}


/**
 * Matches a whole word in str.
 */

uint8_t _match_whole_word( char *str, char *word )
{
    return _match_word_condition(str, word, _char_is_punct_or_space, _char_is_punct_or_space);
}


/**
 * Matches an ending of word.
 */

uint8_t _match_end_of_word( char *str, char *word )
{
    return _match_word_condition(str, word, isalpha, _char_is_punct_or_space);
}


/**
 * Match a word with conditions for the char before and after
 */

uint8_t _match_word_condition(char str[], char word[], int (*comp_before)(int), int (*comp_after)(int))
{
    char *ret = strstr(str, word);
    int before, after;

    while (ret != NULL) {
        before = (int) (ret - str) - 1;
        after = before + strlen(word) + 1;

        /* check if match is a whole word and return true */
        if ( (before == -1 || comp_before(str[before])) &&
             (after == strlen(str) || comp_after(str[after])) )
            return 1;

        /* find next match */
        ret = after != strlen(str) ? strstr(str+after, word) : NULL;
    }

    return 0;
}




int _char_is_punct_or_space(int c)
{
    return ispunct(c) || isspace(c);
}
