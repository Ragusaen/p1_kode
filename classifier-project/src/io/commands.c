#include "io/commands.h"

#define MAX_FN_LEN 255


/* internal functions */
static void _export_command(char *, int (*)(const char**), int, Commands);

static int _command_exit(const char **);
static int _command_help(const char **);
static int _command_train(const char **);
static int _command_test(const char **);
static int _command_threshold(const char **);
static int _command_classify(const char **);

static int _load_dataset_from_arg(const char **, DataSet *, char *);
static int _load_dataset(const char **, DataSet *, int);
static void _config_fallback(const char **, int, char *, char *);
static int _flag_set(const char **, char *);
static int _is_value(const char *);
static void _export_evaluation(const char **, EvaluationSet, char *);
static double _get_threshold(const char **, int);


/**
 * Exports the commands
 */

Commands commands_import(void)
{
    int i = 0;
    Commands exported;

    if ((exported.commands = (Command*) calloc(6, sizeof(Command))) == NULL) fatal_error();

    _export_command("exit", _command_exit, i++, exported);
    _export_command("help", _command_help, i++, exported);
    _export_command("train", _command_train, i++, exported);
    _export_command("test", _command_test, i++, exported);
    _export_command("threshold", _command_threshold, i++, exported);
    _export_command("classify", _command_classify, i++, exported);

    exported.count = i;

    return exported;
}

void _export_command(char name[], int (*func)(const char**), int i, Commands exported) {
    exported.commands[i].func = func;
    strncpy(exported.commands[i].name, name, COMMAND_NAME_LEN);
}


/**
 * Terminates the program.
 * 
 * Command: exit
 */

int _command_exit(const char **argv)
{
    return 0;
}


/**
 * Outputs command guide.
 * 
 * Command: help
 */

int _command_help(const char **argv)
{
    print_command_list();

    return 1;
}


/**
 * Trains the classifier features.
 * 
 * Command: train [path] [--print]
 * 
 * path     path to training dataset (optional, if stored in config)
 * --print  output list to screen
 */

int _command_train(const char **argv)
{
    DataSet training_set;
    FeatureSet trained_features;

    /* load dataset */
    if (!_load_dataset_from_arg(argv, &training_set, "TRAINING_DATASET_PATH"))
        return 1;

    /* train the features - calculates p(CB|F) and p(F) */
    trained_features = train_features(training_set);
    printf("Features successfully trained.\n");

    /* if flag '--print', output the list of features */
    if (_flag_set(argv, "--print") != -1)
        print_trained_features(trained_features);

    return 1;
}


/**
 * Evaluates the performance of the classifier.
 * 
 * Command: test [path] [--print] [--save]
 * 
 * path     path to test dataset (optional, if stored in config)
 * --print  output list to screen
 * --save   export list to file
 */

int _command_test(const char **argv)
{
    DataSet test_set;
    FeatureSet trained_features;
    EvaluationSet evaluation;

    /* load dataset */
    if (!_load_dataset_from_arg(argv, &test_set, "TEST_DATASET_PATH"))
        return 1;

    /* load trained features */
    if (!train_load_features(&trained_features)) {
        error("Features not trained");
        return 1;
    }

    /* score the dataset */
    classifier_score_dataset(test_set, trained_features);

    /* calculate confusion matrix for all thresholds */
    evaluation = test_classifier(test_set);

    /* if flag '--print', output a list of key values */
    if (_flag_set(argv, "--print") != -1)
        print_evaluation(evaluation);

    /* output the ROC-AUC */
    printf("ROC-AUC = %f\n", test_calculate_auc(evaluation));

    /* export the results */
    _export_evaluation(argv, evaluation, "test_results.csv");

    return 1;
}


/**
 * Gets, sets, or calculates threshold.
 * 
 * Command: threshold [number] [--calc [--from]] [--print] [--save]
 * 
 * number   set new threshold (not saved if '--calc' or '--print' is used)
 * --calc    calculate optimal threshold
 *  --from  dataset to use, either 'test' (default) or 'training'
 * --print  output confusion matrix to screen
 * --save   save threshold to config
 */

int _command_threshold(const char **argv)
{
    double threshold;
    char threshold_str[MAX_FN_LEN];
    DataSet dataset;
    FeatureSet trained_features;
    ConfusionMatrix cm;

    /* only load test dataset and trained features if necessary */
    if ((_flag_set(argv, "--calc") != -1 || _flag_set(argv, "--print") != -1) &&
        (!_load_dataset(argv, &dataset, 0) || !train_load_features(&trained_features)))
        return 1;

    if (_flag_set(argv, "--calc") != -1) {
        /* calculate optimal threshold */
        threshold = classifier_calculate_threshold(dataset, trained_features);

        if (_flag_set(argv, "--save") != -1) {
            /* save calculated threshold in config */
            sprintf(threshold_str, "%f", threshold);
            save_config("THRESHOLD", threshold_str);
            printf("Threshold is set to: %f\n", threshold);
        }
        else {
            /* only output the calculated threshold */
            printf("Optimal threshold would be: %f\n", threshold);
        }
    }
    /* get threshold from argv or config */
    else if ((threshold = _get_threshold(argv, 2)) == -1)
        return 1;

    if (_flag_set(argv, "--print") != -1) {
        /* classify the dataset */
        classifier_classify_dataset(dataset, trained_features, threshold);
        /* calculate confusion matrix */
        cm = test_classification(dataset, threshold);
        /* output confusion matrix to screen */
        print_confusion_matrix(cm);
    }

    return 1;
}


/**
 * Classify a single headline. Doesn't work in CLI-loop.
 * 
 * Command:     classify "[Text content of headline]" [--threshold N]
 * 
 * --threshold  set new threshold (N)
 */

int _command_classify(const char **argv)
{
    int cls, i;
    double threshold;
    FeatureSet trained_features;
    Headline headline;

    i = _flag_set(argv, "--threshold");

    if (!_is_value(argv[2]) ||
        !train_load_features(&trained_features) ||
        (threshold = _get_threshold(argv, i != -1 ? (i+1) : -1)) == -1)
        return 1;

    if ((headline.content = (char*) malloc(strlen(argv[2]) + 1)) == NULL ) fatal_error();

    strcpy(headline.content, argv[2]);

    cls = classifier_classify_headline(&headline, trained_features, threshold);

    if (_flag_set(argv, "--vector") != -1)
        print_headline_features(headline.feature_vector, trained_features.count);

    printf(
        "Score: %f\n"
        "Clickbait: %s\n",
        headline.prob_score,
        cls ? "YES" : "NO"
    );

    return 1;
}


/**
 * Load a dataset either from path provided by argv or config
 */

int _load_dataset_from_arg(const char **argv, DataSet *dataset, char *config_key)
{
    
    char filename[MAX_FN_LEN];

    _config_fallback(argv, 2, config_key, filename);

    if (strlen(filename) == 0) {
        error("Dataset path missing");
        return 0;
    }

    return dataset_import(filename, dataset);
    printf("Imported \"%s\", with %d records.\n", filename, dataset->count);
}


/**
 * Load a specific dataset from config, flag '--from' can be 'training', else 'test' is used.
 */

int _load_dataset(const char **argv, DataSet *dataset, int force_training)
{
    int i;
    char filename[MAX_FN_LEN];

    if (force_training == 1 || ((i = _flag_set(argv, "--from")) != -1 && strcmp(argv[i+1], "training") == 0))
        strcpy(filename, load_config("TRAINING_DATASET_PATH"));
    else
        strcpy(filename, load_config("TEST_DATASET_PATH"));

    if (strlen(filename) == 0) {
        error("Dataset path missing");
        return 0;
    }

    return dataset_import(filename, dataset);
}


/**
 * Save or load a config option
 */

void _config_fallback(const char **argv, int i, char *config_key, char *value)
{
    if (_is_value(argv[i])) {
        strcpy(value, argv[i]);
        save_config(config_key, value);
    }
    else {
        strcpy(value, load_config(config_key));
    }
}


/**
 * Check argv for flag
 */

int _flag_set(const char **argv, char *flag)
{
    int i;

    for (i = 2; i < 10 && argv[i] != NULL; i++) {
        if (strcmp(argv[i], flag) == 0) return i;
    }

    return -1;
}


/**
 * Check if argv str is a value
 */

int _is_value(const char *str)
{
    return str != NULL && str[0] != '-';
}


/**
 * Export the evaluation if '--save' flag is set
 */

void _export_evaluation(const char **argv, EvaluationSet evaluation, char* fallback_path)
{
    int i;
    char save_path[MAX_FN_LEN];

    if ((i = _flag_set(argv, "--save")) != -1) {
        /* get or set save path */
        _config_fallback(argv, (i+1), "TEST_SAVE_PATH", save_path);

        /* export file */
        test_export_file(evaluation, strlen(save_path) != 0 ? save_path : fallback_path);
    }
}


/**
 * Get threshold from argv or config
 */

double _get_threshold(const char **argv, int i)
{
    double threshold;
    char threshold_str[MAX_FN_LEN];

    if (i != -1 && _is_value(argv[i])) {
        strcpy(threshold_str, argv[i]);
    }
    else {
        strcpy(threshold_str, load_config("THRESHOLD"));
    }

    if (strlen(threshold_str) == 0) {
        error("No threshold specified");
        return -1;
    }

    /* only save to config if '--save' flag is set or '--print' flag is not set */
    if (_flag_set(argv, "--save") != -1 || _flag_set(argv, "--print") == -1)
        save_config("THRESHOLD", threshold_str);

    /* convert string to double */
    sscanf(threshold_str, "%lf", &threshold);

    printf("Threshold is set to: %f\n", threshold);

    return threshold;
}
