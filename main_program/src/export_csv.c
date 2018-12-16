#include "export_csv.h"

/**
 * Writes a line to a CSV file.
 * 
 * @param fp        a file pointer from fopen with the 'w' flag.
 * @param formats   a string with type format, "%s %d %f %c".
 * @param data      an array of CSV_data unions containing the corresponding type of data
 *                  as the format string specifies.
 */

void write_csv_line(FILE *fp, char *formats, CSV_data *data) {
    int i, format_i = 0, data_i = 0;
    char format[CSV_FORMAT_LEN];

    memset(format, '\0', CSV_FORMAT_LEN);

    for (i = 0; i <= strlen(formats); i++) {
        /* if current character is '%' or white-space, print previous format and data to file */
        if (('%' == formats[i] || isspace(formats[i]) || i == strlen(formats)) && format_i > 0) {
            /* print data to file */
            _print_csv_data(fp, format, data[data_i]);
            data_i++;
            
            /* empty and reset the format string */
            memset(format, '\0', CSV_FORMAT_LEN);
            format_i = 0;

            /* insert comma separator if not at end */
            if (i != strlen(formats))
                fprintf(fp, "%c", CSV_COMMA);
        }

        /* add character to current format string */
        if (!isspace(formats[i]))
            format[format_i++] = formats[i];
    }
    
    /* insert newline */
    fprintf(fp, "\n");
}

void _print_csv_data(FILE *fp, char *format, CSV_data data) {
    int i = (int) strlen(format);
    char temp[CSV_FORMAT_LEN];

    switch (format[i-1]) {
        case 'd':
            fprintf(fp, format, data.d);
            break;
        case 'c':
            fprintf(fp, format, data.c);
            break;
        case 'f':
            /* change format to 's' - _csv_double converts double to string */
            format[i-1] = 's';

            fprintf(fp, format, _csv_double(data.f));
            break;
        case 's':
            /* empty temp string */
            memset(temp, '\0', CSV_FORMAT_LEN);
            /* add a quotation mark to beginning of temp */
            temp[0] = '"';
            /* add format string to temp, usually contains "%s" */
            strcat(temp, format);
            /* add a quotation mark to end of temp */
            temp[i+1] = '"';

            fprintf(fp, temp, data.s);
            break;
    }
}

char* _csv_double(double n) {
    char *str = malloc(16), *token;

    /* convert double to string */
    sprintf(str, "%.6f", n);

    /* replace the decimal separator */
    token = strchr(str, '.');
    token[0] = CSV_DECIMAL;

    return str;
}