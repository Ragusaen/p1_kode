#ifndef EXPORT_CSV_H
#define EXPORT_CSV_H

#include <stdio.h>
#include <stdlib.h>

#define CSV_COMMA ';'
#define CSV_DECIMAL ','
#define CSV_FORMAT_LEN 10

typedef union CSV_data {
    int d; char c; double f; char *s;
} CSV_data;

void write_csv_line(FILE *fp, char *format, CSV_data *data);

char* _csv_double(double n);

#endif