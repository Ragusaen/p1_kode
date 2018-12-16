#ifndef EXPORT_CSV_H
#define EXPORT_CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CSV_COMMA ';'
#define CSV_DECIMAL ','
#define CSV_FORMAT_LEN 10
#define CSV_MAX_STRING_LEN 255

typedef union CSV_data {
    int d; char c; double f; char s[CSV_MAX_STRING_LEN];
} CSV_data;

void write_csv_line(FILE *fp, char *formats, CSV_data *data);

void _print_csv_data(FILE *fp, char *format, CSV_data data);
char* _csv_double(double n);

#endif