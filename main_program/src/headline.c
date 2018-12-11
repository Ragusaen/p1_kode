#include "headline.h"

int import_csv(Headline **headlines, int *headline_count, char *file_path) {
    /* Open file with clickbait headlines */
    FILE *dataset = _open_file(file_path);

    /* Count the number of headlines */
    *headline_count = _count_headlines(dataset);

    /* Allocate memory for the struct */
    if(dataset != NULL) {
        *headlines = (Headline *)malloc(*headline_count * sizeof(Headline));
        if(*headlines == NULL) {
            printf("Error allocating memory for headlines!\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Rewind the file pointer */
    rewind(dataset);
    
    /* Import all data from the file and insert it to the struct */
    _read_headlines(dataset, *headlines, *headline_count);

    return 1;
}

/* ---------- FUNCTIONS ---------- */
/* Open the file and return it to main */
FILE *_open_file( char *file_path_str ) {
    /* Open file with clickbait headlines */
    FILE *dataset = fopen(file_path_str, "r");
    /* Check if the file was opened correctly */
    if(dataset == NULL) {
        printf("Error reading file!\n");
        exit(1);
    }
    return dataset;
}

/* Count the number of headlines and return it to main */
int _count_headlines(FILE *dataset) {
    int lines = 0;
    /* Counts each line for every new line, as to avoid terminating at commas etc. */
    while ( !feof(dataset) ) {
        if ( fgetc(dataset) == '\n' )
            lines++;
    }
    return lines;
}

/* Read the file and insert it to the struct */
void _read_headlines(FILE *dataset, Headline *headlines, int headline_count ) {
    int i;
    /* Gets one full line from the file at a time, until all headlines have been copied to the struct */
    for ( i = 0; i < headline_count; i++ ) {
        char buffer[FILE_BUFFER_LENGTH];
        /* Makes sure the line isn't empty */
        if ( fgets(buffer, FILE_BUFFER_LENGTH, dataset ) ) {
            int j, cb_label = 0;
            for (j=1; !(buffer[j-1] == '\"' && buffer[j] == ',') && j<FILE_BUFFER_LENGTH;j++);
            buffer[j-1] = '\0';
            /* Allocate memory for the each title */
            headlines[i].title = (char*)malloc(j-1);
            if ( headlines[i].title == NULL ) {
                printf("Couldn't allocate memory for headline %d\n", i );
                exit(EXIT_FAILURE);
            }
            /* Copies headline i into the struct */
            strcpy( headlines[i].title, (buffer+1) );

            sscanf(buffer + j + 1, " %d", &cb_label );
            headlines[i].labeled_clickbait = cb_label;
        } else {
            printf("Couldn't read file line %d\n", i );
            exit(EXIT_FAILURE);
        }
    }
}
