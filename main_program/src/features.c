#include "features.h"

uint8_t has_long_word(char str_in[]) {

    int word_length = 0;
    int i = 0;
    char* str_length_pre = str_in,
        *str_length_post = str_in;
    char word_endings[] = " ,;!.:";

//This loop finds the longest word.
    while (i == 0) {
/*defines str_length_post to a pointer to the firs word ending adter str_length_pre. */
        str_length_post = strpbrk(str_length_pre, word_endings);

/*If no word ending is found, is set str_length_post the length of entire string and
the loop ends. */
        if (str_length_post == 0) {
          str_length_post = str_in + strlen(str_in);
          i = 1;
        }
/*If word_legth is smaller than the difference between the last word ending and the new one. */
        if (word_length < strlen(str_length_pre) - strlen(str_length_post)) {
            word_length = strlen(str_length_pre) - strlen(str_length_post);
        }
/*The last word ending is set to the new one */
        str_length_pre = str_length_post + 1;
    }
/*longest word length is cheched against MIN_WORD_LENGTH. if it is bigger the function returns true. */
    return word_length < MIN_WORD_LENGTH;
}

uint8_t has_long_average_words(char str_in[]) {
    int i = 0, sum = 0, words = 0;
    char* str_length_pre = str_in,
    *str_length_post = str_in;
    char word_endings[] = " ,;!.:";

/*Caluculates the individual word lengths like has_long word, but adds the toghether. */
    while (i == 0) {
        str_length_post = strpbrk(str_length_pre, word_endings);

        if (str_length_post == 0) {
            str_length_post = str_in + strlen(str_in);
            i = 1;
        }

        sum += strlen(str_length_pre) - strlen(str_length_post);
        words++;
        str_length_pre = str_length_post + 1;
    }
/*devides the sum with the amount of word, and chechs them against MIN_AVAERAGE_WORD_LENGTH */
    return ((double)sum / words < MIN_AVERAGE_WORD_LENGTH);
}



uint8_t has_special_words(char str_in[]) {
    char* words[AMOUNT_OF_SPECIAL_WORDS] = {
        "sådan", "derfor", "denne", "dette", "her", "så meget", "så lidt"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_SPECIAL_WORDS );
}

uint8_t is_long(char str_in[]) {
/*Sammenligner overskriftens længde med MIN_TOTAL_LENGTH*/
    return (strlen(str_in) < MIN_TOTAL_LENGTH);
}


uint8_t has_punctuation(char str_in[]) {
    char* chars = ":!?";
    return _str_has_chars( str_in, chars );
}

uint8_t has_pronouns(char str_in[]) {
    char *words[AMOUNT_OF_PRONOUNS] = {
        "du", "han", "hun", "hende", "din", "jeg", "os", "de", "min", "dit"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_PRONOUNS );
}

uint8_t has_number(char str_in[]){
    int cb_number = 0, i;

    for(i = 0; i <= strlen(str_in) && cb_number == 0; i++){
        if(isdigit(str_in[i]) ) {
            cb_number = 1;
        }
    }
	return cb_number;
}

uint8_t has_stop_words(char str_in[]) {
    char *words[AMOUNT_OF_STOP_WORDS] = {
        "og", "i", "at", "det", "er", "en", "på", "til", "med", "af", "ikke", "med", "til"
    };
    return _str_count_words( str_in, words, AMOUNT_OF_STOP_WORDS ) > MAX_STOP_WORDS;
}

char * _string_lower( char *str ) {
    int i;
    char* lwr = (char*) malloc( strlen(str) + 1);

    for ( i = 0; i < strlen(str) + 1; i++ ) {
        lwr[i] = tolower(str[i]);
    }

    return lwr;
}

uint8_t _match_whole_word( char *str, char *word ) {
    char *ret = strstr(str, word);
    int before, after;

    if (ret != NULL) {
        before = (int) (ret - str) - 1;
        after = before + strlen(word) + 1;

        return (
            (before == -1 || ispunct(str[before]) || isspace(str[before])) &&
            (after == strlen(str) || ispunct(str[after]) || isspace(str[after]))
        );
    }

    return 0;
}

uint8_t _str_has_chars( char *str, char *chars ) {
    int i;
    for ( i = 0; i < strlen(str); i++ ) {
        int j;
        for ( j = 0; j < strlen(chars); j++ ) {
            if ( str[i] == chars[j] )
                return 1;
        }
    }
    return 0;
}

uint8_t _str_count_words( char *str, char **words, int word_count ) {
    int i, matches = 0;
    char *lower_string = _string_lower(str);

    for ( i = 0; i < word_count; i++ ) {
        if ( _match_whole_word(lower_string, words[i]) )
            matches++;
    }

    free(lower_string);
    return matches;
}
