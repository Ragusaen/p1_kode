#include "features.h"

uint8_t longest_word(char str_in[]) {

    int word_length = 0;
    int i = 0;
    char* str_length_pre = str_in,
        *str_length_post = str_in;
    char word_endings[] = " ,;!.:";

    while (i == 0) {
        str_length_post = strpbrk(str_length_pre, word_endings);

        if (str_length_post == 0) {
          str_length_post = str_in + strlen(str_in);
          i = 1;
        }

        if (word_length < strlen(str_length_pre) - strlen(str_length_post)) {
            word_length = strlen(str_length_pre) - strlen(str_length_post);
        }
        str_length_pre = str_length_post + 1;
    }
    return word_length < MIN_WORD_LENGTH;
}

uint8_t average_length(char str_in[]) {

    int i = 0;
    double sum = 0;
    double words = 0;
    char* str_length_pre = str_in,
    *str_length_post = str_in;
    char word_endings[] = " ,;!.:";

    while (i == 0) {
        str_length_post = strpbrk(str_length_pre, word_endings);

        if (str_length_post == 0) {
            str_length_post = str_in + strlen(str_in);
            i = 1;
        }

        sum += (double) strlen(str_length_pre) - strlen(str_length_post);
        words++;
        str_length_pre = str_length_post + 1;
    }
    return ( sum / words > MAX_AVERAGE_WORD_LENGTH);
}



uint8_t special_words(char str_in[]) {
    int has_word = 0;
    int i = 0;

    char* words[AMOUNT_OF_SPECIAL_WORDS] = {
        "sådan", "derfor", "denne", "dette", "her", "så meget", "så lidt"
    };

    char *lower_string = _string_lower(str_in);

    for ( i = 0; i < AMOUNT_OF_SPECIAL_WORDS && !has_word; i++ ) {
        has_word = _match_whole_word(lower_string, words[i]);
    }

    free(lower_string);
    
    return has_word;
}

uint8_t total_length(char str_in[]) {
    return (strlen(str_in) < MIN_TOTAL_LENGTH);
}


uint8_t has_cb_punctuation(char str_in[]) {
   const char ch = '.';
   char *pstrchr = strchr(str_in, ch);

   /*Hvis beskeden indeholder charateren*/
   return (pstrchr != NULL);
}

uint8_t has_cb_hastag(char str_in[]) {
   const char ch = '#';
   char *pstrchr = strchr(str_in, ch);

   /*Hvis beskeden indeholder charateren*/
   return (pstrchr != NULL);
}

uint8_t begins_with_number(char str_in[]){
    int cb_number = 0, i;

    for(i = 0; i <= 3 && cb_number == 0; i++){
        if(isdigit(str_in[i]) ) {
            cb_number = 1;
        }
    }
	return cb_number;
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