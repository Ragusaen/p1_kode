#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorhandler.h"
#include "config.h"
#include "classifier.h"
#include "headline.h"
#include "features.h"
#include "train.h"
#include "evaluation.h"


#define COMMAND_NAME_LEN 10
#define MAX_FN_LEN 255


typedef struct Command {
    char name[COMMAND_NAME_LEN];
    int (*func)(const char**);
} Command;

typedef struct Commands {
    Command *commands;
    int count;
} Commands;


Commands import_commands();
void _export_command(char name[], int (*func)(const char**), int i, Commands exported);

int c_exit(const char **argv);
int c_help(const char **argv);
int c_train(const char **argv);
int c_test(const char **argv);
int c_threshold(const char **argv);
int c_classify(const char **argv);


int _load_dataset_from_arg(const char **argv, DataSet *dataset, char *config_key);
int _load_dataset(const char **argv, DataSet *dataset, int force_training);
void _config_fallback(const char **argv, int i, char *config_key, char *value);
int _flag_set(const char **argv, char *flag);
int _is_value(const char *str);
void _export_evaluation(const char **argv, EvaluationSet evaluation, char* fallback_path);
double _get_threshold(const char **argv, int i);

void _print_trained_features(FeatureSet featureset);
void _print_feature(Feature feature);
void _print_evaluation(EvaluationSet evaluation);
void _print_confusion_matrix(ConfusionMatrix cm);
void _print_key_values_header();
void _print_key_values(ConfusionMatrix cm);
void _print_headline_features(uint8_t feature_vector, int count);
void _print_thin_line();
void _print_thick_line();

#endif