#include "core/headline.h"

#define FILE_BUFFER_LENGTH 256

/* internal functions */
static int _count_headlines(FILE *);
static void _read_headlines(FILE *, DataSet);
static void _parse_headline(Headline *, char *, char *);


int headline_import_dataset(char file_path[], DataSet *dataset)
{
    FILE *fp;

    /* Open file with clickbait headlines */
    if((fp = fopen(file_path, "r")) == NULL) {
        printf("Couldn't read file \"%s\"\n", file_path);
        return 0;
    }

    /* Count the number of headlines */
    dataset->count = _count_headlines(fp);

    /* Allocate memory for the struct */
    if ((dataset->data = (Headline *) malloc(dataset->count * sizeof(Headline))) == NULL) fatal_error();
    
    /* Import all data from the file and insert it to the struct */
    _read_headlines(fp, *dataset);

    return 1;
}


/**
 * Count the number of headlines and return it to main
 */

int _count_headlines(FILE *fp)
{
    int lines = 0;

    /* Counts each line for every new line and until EOF, as to avoid terminating at commas etc. */
    while (!feof(fp)) {
        if (fgetc(fp) == '\n')
            lines++;
    }

    /* Rewind the file pointer */
    rewind(fp);

    return lines;
}

/**
 * Read the file and insert it to the struct
 */

void _read_headlines(FILE *fp, DataSet dataset)
{
    int i = 0;
    char buffer[FILE_BUFFER_LENGTH], *token;

    /* Gets one full line from the file at a time, until all headlines have been copied to the struct */
    while (fgets(buffer, FILE_BUFFER_LENGTH, fp) != NULL) {
        if ((token = strchr(buffer, '\t')) == NULL) fatal("Couldn't parse line in dataset");

        _parse_headline(dataset.data + i, buffer, token); i++;
    }
}

void _parse_headline(Headline *headline, char *buffer, char *token)
{
    int len, cb_label;
    
    len = (int) strlen(buffer) - strlen(token);

    /* Allocate memory for the content */
    if ((headline->content = (char*) malloc(len + 1)) == NULL ) fatal_error();

    /* Copies headline i into the struct */
    strncpy(headline->content, buffer, len);
    headline->content[len] = '\0';

    sscanf(buffer + len, " %d", &cb_label);
    headline->labeled_clickbait = cb_label;
}
