#include "config.h"

char* load_config(const char *key)
{
    char buffer[CONFIG_LINE_LEN];
    FILE *fp;

    if ((fp = fopen(CONFIG_FN, "r")) == NULL) return "";

    while (fgets(buffer, CONFIG_LINE_LEN, fp) != NULL) {
        if (strcmp(key, _get_config_key(buffer)) == 0) {
            return _get_config_value(buffer);
        }
    }

    return "";
}


int save_config(const char *key, const char *value)
{
    int wrote_config = 0;
    char buffer[CONFIG_LINE_LEN], output[CONFIG_MAX_FILESIZE] = "";
    FILE *fp;

    if ((fp = fopen(CONFIG_FN, "ab+")) == NULL) {
        error("Couldn't save config");
        return 0;
    }

    while (fgets(buffer, CONFIG_LINE_LEN, fp) != NULL) {
        if (strcmp(key, _get_config_key(buffer)) == 0) {
            sprintf(output + strlen(output), "%s %s\n", key, value);
            wrote_config = 1;
        }
        else {
            buffer[strlen(buffer)-2] = '\0';
            sprintf(output + strlen(output), "%s\n", buffer);
        }
    }

    if (wrote_config == 0)
        sprintf(output + strlen(output), "%s %s\n", key, value);

    fclose(fp);

    if ((fp = fopen(CONFIG_FN, "w")) == NULL) {
        error("Couldn't save config");
        return 0;
    }

    fprintf(fp, "%s", output);

    fclose(fp);

    return 1;
}


char* _get_config_key(char *line)
{
    int len;
    char *key, *token;

    if ((token = strchr(line, ' ')) == NULL)
        return "";

    len = (int) strlen(line) - strlen(token);

    if ((key = (char*) malloc(len + 1)) == NULL ) fatal_error();
    
    strncpy(key, line, len);
    key[len] = '\0';

    return key;
}

char* _get_config_value(char *line)
{
    int len;
    char *value, *token;

    if ((token = strchr(line, ' ')) == NULL)
        return "";

    len = (int) strlen(line) - strlen(token);

    if ((value = (char*) malloc(strlen(token - 1))) == NULL ) fatal_error();

    strncpy(value, line + len + 1, strlen(token - 2));
    value[strlen(value) - 1] = '\0';

    return value;
}