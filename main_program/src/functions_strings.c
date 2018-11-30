#include "functions_strings.h"


int laengste_ord(char str_in[]) {

  int ordlaengde = 0;
  int i = 0;
  char* str_laengde_pre = str_in,
        *str_laengde_post = str_in;
  char ord_endelser[] = " ,;!.:";
 
  while (i == 0) {
    str_laengde_post = strpbrk(str_laengde_pre, ord_endelser);

    if (str_laengde_post == NULL) {
      str_laengde_post = str_in + strlen(str_in);
      i = 1;
    }

    if (ordlaengde < strlen(str_laengde_pre) - strlen(str_laengde_post)) {
      ordlaengde = strlen(str_laengde_pre) - strlen(str_laengde_post);
    }  
    str_laengde_pre = str_laengde_post + 1;
  }
  return ordlaengde;

}

double gaennemsnit_laengde(char str_in[]) {
  
  int gennemsnit = 0;
  int i = 0;
  double sum = 0;
  double ord = 0;
  char* str_laengde_pre = str_in,
    *str_laengde_post = str_in;
  char ord_endelser[] = " ,;!.:";

  while (i == 0) {
    str_laengde_post = strpbrk(str_laengde_pre, ord_endelser);

    if (str_laengde_post == NULL) {
      str_laengde_post = str_in + strlen(str_in);
      i = 1;
    }

    sum += (double) strlen(str_laengde_pre) - strlen(str_laengde_post);
    ord++;
    str_laengde_pre = str_laengde_post + 1; 
  }
  return (double) sum / ord;

}
  


int bestemte_ord(char str_in[]) {
  const char* ord[2];
  ord[0] = "IN";
  ord[1] = "YEET";
  ord[3] = "YUUYT";
  ord[4] = "YAATY";
  ord[5] = "EYYYT";
  int ord_maengde = 5;
  int ord_score = 0;
  int i = 0;

  for (i; i <= ord_maengde; i++) {
    printf("YUET I %d \n", i);
    if (strstr(str_in, ord[i]) != 0) {
      printf("YT \n");
      ord_score++;
    }
  }
  return ord_score;

}