#include "features.h"

/**
 * Checks if headline only contains words less than 8 characters long.
 */

uint8_t has_no_long_word(char str_in[]) {
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

uint8_t has_low_average_word_length(char str_in[]) {
    int i = 0,
        word_length_sum = 0,
        word_count = 0,
        word_length = 0;

    for (i = 0; i <= strlen(str_in); i++) {
        /* if end-of-str or current char is punctuation or white-space */
        if (i == strlen(str_in) || ispunct(str_in[i]) || isspace(str_in[i])) {
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
 * Checks if headline contains special forward-referencing words.
 */

uint8_t has_special_words(char str_in[]) {
    char* words[AMOUNT_OF_SPECIAL_WORDS] = {
        "sådan", "derfor", "denne", "dette", "her", "så meget", "så lidt"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_SPECIAL_WORDS ) > 0;
}


/**
 * Checks if headline is less than 40 characters long.
 */

uint8_t is_short(char str_in[]) {
    return strlen(str_in) < MIN_TOTAL_LENGTH;
}


/**
 * Checks if headline contains special punctuation, : ! ?
 */

uint8_t has_special_punctuation(char str_in[]) {
    return strpbrk(str_in, ":!?") != NULL;
}


/**
 * Checks if headline contains pronouns.
 */

uint8_t has_pronouns(char str_in[]) {
    char *words[AMOUNT_OF_PRONOUNS] = {
        "du", "han", "hun", "hende", "din", "jeg", "os", "de", "min", "dit"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_PRONOUNS ) > 0;
}


/**
 * Checks if headline contains a number.
 */

uint8_t has_number(char str_in[]) {
	return strpbrk(str_in, "0123456789") != NULL;
}


/**
 * Checks if headline contains more than 2 stop-words.
 */

uint8_t has_stop_words(char str_in[]) {
    char *words[AMOUNT_OF_STOP_WORDS] = {
        "og", "i", "at", "det", "er", "en", "på", "til", "med", "af", "ikke", "med", "til"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_STOP_WORDS ) > MAX_STOP_WORDS;
}


/**
 * Transforms str to lowercase.
 */

char * _string_lower( char *str ) {
    int i;
    char* lwr = (char*) malloc( strlen(str) + 1);

    for ( i = 0; i < strlen(str) + 1; i++ ) {
        lwr[i] = tolower(str[i]);
    }

    return lwr;
}


/**
 * Matches a whole word in str.
 */

uint8_t _match_whole_word( char *str, char *word ) {
    char *ret = strstr(str, word);
    int before, after;

    while (ret != NULL) {
        before = (int) (ret - str) - 1;
        after = before + strlen(word) + 1;

        /* check if match is a whole word and return true */
        if ( (before == -1 || ispunct(str[before]) || isspace(str[before])) &&
             (after == strlen(str) || ispunct(str[after]) || isspace(str[after])) )
            return 1;
        
        /* find next match */
        ret = after != strlen(str) ? strstr(str+after, word) : NULL;
    }

    return 0;
}


/**
 * Counts matched whole words in str.
 */

int _str_count_words( char *str, char **words, int word_count ) {
    int i, matches = 0;
    char *lower_string = _string_lower(str);

    for ( i = 0; i < word_count; i++ ) {
        if ( _match_whole_word(lower_string, words[i]) )
            matches++;
    }

    free(lower_string);
    return matches;
}
