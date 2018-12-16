#include "headline.h"

DataSet import_headline_csv(char file_path[]) {
    FILE *fp;
    DataSet dataset;

    /* Open file with clickbait headlines */
    if((fp = fopen(file_path, "r")) == NULL) {
        printf("Error reading file \"%s\".\n", file_path);
        exit(EXIT_FAILURE);
    }

    /* Count the number of headlines */
    dataset.count = _count_headlines(fp);

    /* Allocate memory for the struct */
    if ((dataset.data = (Headline *) malloc(dataset.count * sizeof(Headline))) == NULL) {
        printf("Error allocating memory for headlines.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Import all data from the file and insert it to the struct */
    _read_headlines(fp, dataset);

    return dataset;
}


/**
 * Count the number of headlines and return it to main
 */

int _count_headlines(FILE *dataset) {
    int lines = 0;

    /* Counts each line for every new line and until EOF, as to avoid terminating at commas etc. */
    while ( !feof(dataset) ) {
        if ( fgetc(dataset) == '\n' )
            lines++;
    }

    /* Rewind the file pointer */
    rewind(dataset);

    return lines;
}

/**
 * Read the file and insert it to the struct
 */

void _read_headlines(FILE *fp, DataSet dataset) {
    int i;

    /* Gets one full line from the file at a time, until all headlines have been copied to the struct */
    for ( i = 0; i < dataset.count; i++ ) {
        char buffer[FILE_BUFFER_LENGTH];
        /* Makes sure the line isn't empty */
        if ( fgets(buffer, FILE_BUFFER_LENGTH, fp) ) {
            int j, cb_label = 0;
            for (j=1; !(buffer[j-1] == '\"' && buffer[j] == ',') && j<FILE_BUFFER_LENGTH;j++);
            buffer[j-1] = '\0';
            /* Allocate memory for the each title */
            dataset.data[i].title = (char*)malloc(j-1);
            if ( dataset.data[i].title == NULL ) {
                printf("Couldn't allocate memory for headline %d\n", i );
                exit(EXIT_FAILURE);
            }
            /* Copies headline i into the struct */
            strcpy( dataset.data[i].title, (buffer+1) );

            sscanf(buffer + j + 1, " %d", &cb_label );
            dataset.data[i].labeled_clickbait = cb_label;
        }
        else {
            printf("Couldn't read file line %d\n", i );
            exit(EXIT_FAILURE);
        }
    }
}
