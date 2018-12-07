#include "headline.h"
int import_csv(Headline **headlines, int *headline_count, char *file_path) {
    /* Declare variables */
    int headlines_count = 0;

    /* Open file with clickbait headlines */
    FILE *dataset = open_file(file_path);

    /* Count the number of headlines */
    headlines_count = count_headlines(dataset);

    /* Allocate memory for the struct */
    if(dataset != NULL) {
        *headlines = (Headline *)malloc(headlines_count * sizeof(Headline));
        if(*headlines == NULL) {
           exit(EXIT_FAILURE);
        }
    }

    /* Rewind the file pointer */
    rewind(dataset);

    /* Import all data from the file and insert it to the struct */
    read_headlines(dataset, *headlines);

    /* Return headlines_count to main */
    headline_count = &headlines_count;

    return 1;
}

/* ---------- FUNCTIONS ---------- */
/* Open the file and return it to main */
FILE *open_file( char *file_path_str ) {
    /* Open file with clickbait headlines */
    FILE *dataset = fopen( file_path_str, "r");
    /* Check if the file was opened correctly */
    if(dataset == NULL) {
        printf("\nError reading file.\n\n");
        exit(1);
    }
    return dataset;
}

/* Count the number of headlines and return it to main */
int count_headlines(FILE *dataset) {
    char buf_count[MAX_HEADLINE_LENGTH];
    int i = 0;

    while(fgets(buf_count, MAX_HEADLINE_LENGTH, dataset)) {
        i++;
    }
    return i;
}

/* Read the file and insert it to the struct */
void read_headlines(FILE *dataset, Headline *headlines) {
    char buf_headline[MAX_HEADLINE_LENGTH],
    buf_clickbait[2],
    full_line[MAX_HEADLINE_LENGTH];
    char *section;
    const char exclude_symbols[] = ";";
    int i = 0;
    Headline headline;

    while(fgets(full_line, MAX_HEADLINE_LENGTH, dataset)) {
        section = strtok(full_line, exclude_symbols);
        if(section != NULL) {
            strcpy(buf_headline, section);
            section = strtok(NULL, exclude_symbols);
            strcpy(buf_clickbait, (section + 1));
        }
        /* Add buffers to struct */
        strcpy(headline.title, buf_headline);
        headline.labeled_clickbait = atoi(buf_clickbait);
        headlines[i] = headline;
        i++;
    }
}
