#include "clickbait_database.h"
int import_csv() {
  /* Declare variables */
  Headlines *headlines;
  int i = 1;
  int j = 0;
  char headline[MAX_HEADLINE_LENGTH], line[MAX_HEADLINE_LENGTH];
  int headlines_count = 0;
  char *clickbait_or_not;

  /* Open file with clickbait headlines */
  FILE *dataset = open_file();

  /* Count the number of headlines */
  headlines_count = count_headlines(dataset);
  printf("Number of headlines: %d", headlines_count);

  /* Allocate memory for the struct */
  if(dataset != NULL) {
    headlines = (Headlines *)malloc(headlines_count * sizeof(Headlines));
    if(headlines == NULL) {
      printf("Failed to allocate memory. Have a pleasant day.\n");
      exit(EXIT_FAILURE);
    }
    else {
      printf("Memory allocated successfully.\n");
    }
  }

  /* Rewind the file pointer */
  rewind(dataset);

  /* Import all data from the file and insert it to the struct */
  read_headlines(dataset, headlines);

printf("\n----------Import complete----------\n");
return 1;
}

/* ---------- FUNCTIONS ---------- */
/* Open the file and return it to main */
FILE *open_file() {
  /* Åben databasen med clickbait headlines */
  FILE *dataset = fopen("headlines.csv", "r");
  /* Check om filen blev åbnet korrekt */
  if(dataset == NULL) {
    printf("\nError reading file.\n\n");
    exit(1);
  }
  return dataset;
}

/* Count the number of headlines and return it to main */
int count_headlines(FILE *dataset) {
  char buf_count[MAX_HEADLINE_LENGTH];
  int count, i = 0;

  while(fgets(buf_count, MAX_HEADLINE_LENGTH, dataset)) {
    i++;
  }
  return i;
}

/* Read the file and insert it to the struct */
void read_headlines(FILE *dataset, Headlines headlines[]) {
  char buf_headline[MAX_HEADLINE_LENGTH],
  buf_clickbait[2],
  full_line[MAX_HEADLINE_LENGTH];
  char *section;
  const char exclude_symbols[] = "\"";
  Headlines headline;

  while(fgets(full_line, MAX_HEADLINE_LENGTH, dataset)) {
    section = strtok(full_line, exclude_symbols);
    if(section != NULL) {
      strcpy(buf_headline, section);
      section = strtok(NULL, exclude_symbols);
      strcpy(buf_clickbait, (section + 1));
    }
    /* Add buffers to struct */
    strcpy(headlines.headline, buf_headline);
    strcpy(headlines.headline, buf_name);
    headlines[i] = headline;
    i++
  }
}
