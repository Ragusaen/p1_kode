#include "config.h"

#define CONFIG_FN "cbclassifier.config"
#define CONFIG_LINE_LEN 255
#define CONFIG_MAX_FILESIZE 1024

/* internal functions */
static char* _get_config_part(int, char *);
static char* _get_substring(char *, int);


/**
 * Gets a value stored with 'key'
 */

char* load_config(const char *key)
{
    char buffer[CONFIG_LINE_LEN];
    FILE *fp;

    if ((fp = fopen(CONFIG_FN, "r")) == NULL) return "";

    while (fgets(buffer, CONFIG_LINE_LEN, fp)) {
        if (strcmp(key, _get_config_part(0, buffer)) == 0) {
            return _get_config_part(1, buffer);
        }
    }

    return "";
}


/**
 * Saves or overwrites a key and value pair
 */

int save_config(const char *key, const char *value)
{
    int len, saved = 0;
    char buffer[CONFIG_LINE_LEN], output[CONFIG_MAX_FILESIZE], *n;
    FILE *fp;

    memset(output, '\0', CONFIG_MAX_FILESIZE);

    /* first, open config file for reading (created if not existing) */
    if ((fp = fopen(CONFIG_FN, "ab+")) == NULL) {
        error("Couldn't save config");
        return 0;
    }

    /* read all lines of config file, skip line matching key */
    while (fgets(buffer, CONFIG_LINE_LEN, fp)) {
        if (strcmp(key, _get_config_part(0, buffer)) != 0) {
            len = (int) strlen(buffer);

            /* remove newline characters */
            if ((n = strpbrk(buffer, "\r\n")) != NULL)
                len -= (int) strlen(n);

            sprintf(output + strlen(output), "%s\n", _get_substring(buffer, len));
        }
        else {
            sprintf(output + strlen(output), "%s\t%s\n", key, value);
            saved = 1;
        }
    }

    if (!saved)
        sprintf(output + strlen(output), "%s\t%s\n", key, value);

    output[strlen(output)] = '\0';

    fclose(fp);

    /* second, open config file again, but for writing */
    if ((fp = fopen(CONFIG_FN, "w")) == NULL) {
        error("Couldn't save config");
        return 0;
    }

    /* write output to file */
    fprintf(fp, "%s", output);

    fclose(fp);

    return 1;
}


/**
 * Parses 'key' from line
 */

char* _get_config_part(int part, char *line)
{
    int len;
    char *value, *token, *n;

    if ((token = strchr(line, '\t')) == NULL)
        return "";

    if (part == 0) {
        len = (int) strlen(line) - strlen(token);
        value = _get_substring(line, len);
    }
    else {
        len = (int) strlen(token) - 1;

        /* remove newline characters */
        if ((n = strpbrk(line, "\r\n")) != NULL)
            len -= (int) strlen(n);

        value = _get_substring(token + 1, len);
    }

    return value;
}


char* _get_substring(char *str, int length)
{
    char *substr;

    if ((substr = (char*) malloc(length + 1)) == NULL ) fatal_error();
    
    strncpy(substr, str, length);
    substr[length] = '\0';

    return substr;
}