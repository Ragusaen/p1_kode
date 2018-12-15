#include "export_csv.h"

void write_csv_line(FILE *fp, char *formats, CSV_data *data) {
    int i, format_i = 0, data_i = 0;
    char format[CSV_FORMAT_LEN], temp[CSV_FORMAT_LEN];

    memset(format, '\0', CSV_FORMAT_LEN);
    memset(temp, '\0', CSV_FORMAT_LEN);

    for (i = 0; i <= strlen(formats); i++) {
        if (formats[i] == '%' || i == strlen(formats)) {
            if (format_i > 0) {
                switch (formats[i-1]) {
                    case 'd': fprintf(fp, format, data[data_i].d); break;
                    case 'c': fprintf(fp, format, data[data_i].c); break;
                    case 'f': format[format_i-1] = 's';
                              fprintf(fp, format, _csv_double(data[data_i].f)); break;
                    case 's': format[format_i] = '"'; temp[0] = '"'; strcat(temp, format);
                              fprintf(fp, temp, data[data_i].s);
                              memset(temp, '\0', CSV_FORMAT_LEN); break;
                }

                if (i == strlen(formats))
                    fprintf(fp, "\n");
                else
                    fprintf(fp, "%c", CSV_COMMA);

                data_i++; format_i = 0;
                memset(format, '\0', CSV_FORMAT_LEN);
            }
        }

        format[format_i++] = formats[i];
    }
}

char* _csv_double(double n) {
    char *str = malloc(16), *token;

    sprintf(str, "%.6f", n);

    /* replace '.' with ',' */
    token = strchr(str, '.');
    token[0] = CSV_DECIMAL;

    return str;
}