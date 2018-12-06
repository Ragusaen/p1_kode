#include "functions_strings.h"


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
  return word_length;

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
  return (double) sum / words;
}



int special_words(char str_in[]) {
  int words_amount = 5;
  const char* words[words_amount];
  words[0] = "IN";
  words[1] = "YEET";
  words[3] = "YUUYT";
  words[4] = "YAATY";
  words[5] = "EYYYT";
  int word_score = 0;
  int i = 0;

  for (i; i <= words_amount; i++) {
    if (strstr(str_in, words[i]) != 0) {
      word_score++;
    }
  }
  return word_score;

}

int total_length(char str_in[]){
  return strlen(str_in);
}