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

double average_length(char str_in[]) {

    int average = 0;
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
    if(((double) sum / words) > MAX_AVERAGE_WORD_LENGTH){
    }
    else{
        return 1;
    }
}



int special_words(char str_in[]) {
    int is_word = 0;
    int i = 0;

    char* words[AMOUNT_OF_SPECIAL_WORDS];
    words[0] = "IN";
    words[1] = "YEET";
    words[3] = "YUUYT";
    words[4] = "YAATY";
    words[5] = "EYYYT";

    for (i; i <= AMOUNT_OF_SPECIAL_WORDS; i++) {
        if (strstr(str_in, words[i]) != 0) {
        is_word++;
        break;
        }
    }
    return is_word;
}

int total_length(char str_in[]){
    if(strlen(str_in) > MAX_TOTAL_LENGTH){
        return 0;
    }
    else{
        return 1;
    }
}


/* Alternativ, hvis der onskes tjekke for om en hvis maengde er tilstede i beskeden udskift if else i funktionen med nederstaaende*/
/*
   const char ch = 'acharacter'; 
   int toomuch =3;
   int count =0;
   int cb_something;
   int i;

   for(i =0; i <= strlen(str_in); i++) {
       if(str_in[i] == ch){
           count++;
       }
   }
   if (count >= toomuch) {
       cb_something =1;
   }
   else{
       cb_something =0;
   }
*/
   /* END */

int has_cb_punctuation(char str_in[]) {
   const char ch = '.';
   int cb_punctuation;
   char *pstrchr;
   pstrchr = strchr(str_in, ch);

   /*Hvis beskeden indeholder charateren*/
   if(pstr != NULL) {
	   cb_punctuation = 1;
   }
   else{
	   cb_punctuation = 0;
   }
   return cb_punctuation;
}
int has_cb_hastag(char str_in[]) {
   const char ch = '#';
   int cb_hastag;
   char *pstrchr;
   pstrchr = strchr(str_in, ch);

   /*Hvis beskeden indeholder charateren*/
   if(pstr != NULL) {
	   cb_hastag = 1;
   }
   else{
	   cb_hastag = 0;
   }
   return cb_hastag;
}

int begins_with_number(char str_in[]){
    int cb_number;
	
    for(i =0; i <=3; i++){
        if(isdigit(str_in[i])) {
            cb_number =1;
        }
        else if(cb_number ==1) {
            cb_number =1;
	    }
        else {
            cb_number =0;
	    }
    }	
	return cb_number;
}