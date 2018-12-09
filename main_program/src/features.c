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
    if(word_length > MAX_WORD_LENGTH){
        return 0;
    }
    else{
        return 1;
    }
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
    return ((double) sum / words > MAX_AVERAGE_WORD_LENGTH);
}


uint8_t special_words(char str_in[]) {
    int has_word = 0;
    int i;

    char* words[AMOUNT_OF_SPECIAL_WORDS];
    words[0] = "sådan";
    words[1] = "YEET";
    words[2] = "WhoDis";
    words[3] = "YUUYT";
    words[4] = "YAATY";
    words[5] = "EYYYT";

    for ( i = 0; i < AMOUNT_OF_SPECIAL_WORDS && !has_word; i++ ) {
        if (strstr(str_in, words[i]) != 0) {
            has_word = 1;
        }
    }
    return has_word;
}

uint8_t total_length(char str_in[]) {
    return (strlen(str_in) > MAX_TOTAL_LENGTH);
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
