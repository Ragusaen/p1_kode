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

    while (fgets(buffer, CONFIG_LINE_LEN, fp) != NULL) {
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
    int wrote_config = 0;
    char buffer[CONFIG_LINE_LEN], output[CONFIG_MAX_FILESIZE] = "";
    FILE *fp;

    /* first, open config file for reading (created if not existing) */
    if ((fp = fopen(CONFIG_FN, "ab+")) == NULL) {
        error("Couldn't save config");
        return 0;
    }

    /* read all lines of config file */
    while (fgets(buffer, CONFIG_LINE_LEN, fp) != NULL) {
        if (strcmp(key, _get_config_part(0, buffer)) == 0) {
            /* overwrite */
            sprintf(output + strlen(output), "%s %s\n", key, value);
            wrote_config = 1;
        }
        else {
            /* remove '\n' before end of line */
            buffer[strlen(buffer)-2] = '\0';
            /* copy to output */
            sprintf(output + strlen(output), "%s\n", buffer);
        }
    }

    /* key wasn't found in config, append */
    if (wrote_config == 0)
        sprintf(output + strlen(output), "%s %s\n", key, value);

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
    char *value, *token;

    if ((token = strchr(line, ' ')) == NULL)
        return "";

    if (part == 0)
        value = _get_substring(line, (int) strlen(line) - strlen(token));
    else
        value = _get_substring(token + 1, strlen(token) - 2);

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