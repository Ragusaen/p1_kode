#include "commands.h"

/**
 * Exports the commands
 */

Commands import_commands()
{
    int i = 0;
    Commands exported;

    if ((exported.commands = (Command*) calloc(6, sizeof(Command))) == NULL) fatal_error();

    _export_command("exit", c_exit, i++, exported);
    _export_command("help", c_help, i++, exported);
    _export_command("train", c_train, i++, exported);
    _export_command("test", c_test, i++, exported);
    _export_command("threshold", c_threshold, i++, exported);
    _export_command("classify", c_classify, i++, exported);

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

int c_exit(const char **argv)
{
    return 0;
}


/**
 * Outputs command guide.
 * 
 * Command: help
 */

int c_help(const char **argv)
{
    printf("\nCOMMANDS\n");
    _print_thick_line();
    printf(
        "train [path]\tTrain features on training set (path optional, if in config)\n"
        "\n"
        "\tFlags:\t--print:       print trained features\n"
    );
    _print_thin_line();
    printf(
        "test [path]\tRun evaluation on test set (path optional, if in config)\n"
        "\n"
        "\tFlags:\t--print:       print evaluation data\n"
        "\t      \t--save [path]: export CSV data\n"
    );
    _print_thin_line();
    printf(
        "threshold [number]\tGet saved threshold, number only used if flags set\n"
        "\n"
        "\tSubr.:\t-calc:            calculate optimal threshold\n"
        "\n"
        "\tFlags:\t--from [dataset]: either 'test' (default) or 'training'\n"
        "\t      \t--print:          print confusion matrix based on threshold\n"
        "\t      \t--save:           save threshold to config\n"
    );
    _print_thin_line();
    printf(
        "classify \"text\" [--threshold N]\tClassify a single headline (text)\n"
        "\n"
        "\tFlags:\t--threshold N:    set new threshold (N)\n"
    );
    _print_thick_line();

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

int c_train(const char **argv)
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
        _print_trained_features(trained_features);

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

int c_test(const char **argv)
{
    DataSet test_set;
    FeatureSet trained_features;
    EvaluationSet evaluation;

    /* load dataset */
    if (!_load_dataset_from_arg(argv, &test_set, "TEST_DATASET_PATH"))
        return 1;

    /* load trained features */
    if (!load_trained_features(&trained_features)) {
        error("Features not trained");
        return 1;
    }

    /* score the dataset */
    score_dataset(test_set, trained_features);

    /* calculate confusion matrix for all thresholds */
    evaluation = evaluate_classifier(test_set);

    /* if flag '--print', output a list of key values */
    if (_flag_set(argv, "--print") != -1)
        _print_evaluation(evaluation);

    /* output the ROC-AUC */
    printf("ROC-AUC = %f\n", calculate_AUC(evaluation));

    /* export the results */
    _export_evaluation(argv, evaluation, "test_results.csv");

    return 1;
}


/**
 * Gets, sets, or calculates threshold.
 * 
 * Command: threshold [number] [-calc [--from]] [--print] [--save]
 * 
 * number   set new threshold (not saved if '-calc' or '--print' is used)
 * -calc    calculate optimal threshold
 *  --from  dataset to use, either 'test' (default) or 'training'
 * --print  output confusion matrix to screen
 * --save   save threshold to config
 */

int c_threshold(const char **argv)
{
    double threshold;
    char threshold_str[MAX_FN_LEN];
    DataSet dataset;
    FeatureSet trained_features;
    ConfusionMatrix cm;

    /* only load test dataset and trained features if necessary */
    if ((_flag_set(argv, "-calc") != -1 || _flag_set(argv, "--print") != -1) &&
        (!_load_dataset(argv, &dataset, 0) || !load_trained_features(&trained_features)))
        return 1;

    if (_flag_set(argv, "-calc") != -1) {
        /* calculate optimal threshold */
        threshold = calculate_threshold(dataset, trained_features);

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
        classify_dataset(dataset, trained_features, threshold);
        /* calculate confusion matrix */
        cm = evaluate_classification(dataset, threshold);
        /* output confusion matrix to screen */
        _print_confusion_matrix(cm);
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

int c_classify(const char **argv)
{
    int cls, i;
    double threshold;
    FeatureSet trained_features;
    Headline headline;

    i = _flag_set(argv, "--threshold");

    if (!_is_value(argv[2]) ||
        !load_trained_features(&trained_features) ||
        (threshold = _get_threshold(argv, i != -1 ? (i+1) : -1)) == -1)
        return 1;

    if ((headline.content = (char*) malloc(strlen(argv[2]) + 1)) == NULL ) fatal_error();

    strcpy(headline.content, argv[2]);

    cls = classify(&headline, trained_features, threshold);

    printf("Score: %f\tClickbait?  %s\n", headline.prob_score, cls ? "YES" : "NO");

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

    *dataset = import_headline_csv(filename);
    printf("Imported \"%s\", with %d records.\n", filename, dataset->count);

    return 1;
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

    *dataset = import_headline_csv(filename);

    return 1;
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
        write_evaluation_file(evaluation, strlen(save_path) != 0 ? save_path : fallback_path);
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


/**
 * Outputs a list of trained features
 */

void _print_trained_features(FeatureSet featureset)
{
    uint8_t i;
    printf("\n%-23s %10s %10s %10s\n", "Feature", "p(CB|F)", "p(CB|!F)", "p(F)");

    for ( i = 0; i < featureset.count; i++ ) {
        _print_feature(featureset.features[i]);
    }
}

void _print_feature(Feature feature)
{
    printf("%-23s %10.4f %10.4f %10.4f\n",
        feature.name,
        feature.prob_cb_given_feature,
        ( 0.5 - feature.prob_cb_given_feature * feature.prob_feature ) / ( 1 - feature.prob_feature ),
        feature.prob_feature
    );
}


/**
 * Prints a list of key values from an evaluation set (array of confusion matrixes)
 */

void _print_evaluation(EvaluationSet evaluation)
{
    int i;

    printf("\nCLASSIFIER EVALUATION\n");
    _print_key_values_header();

    for (i = 0; i < evaluation.count; i++) {
        _print_key_values(evaluation.data[i]);
    }

    _print_thick_line();
}


/**
 * Prints a confusion matrix, with all associated scores
 */

void _print_confusion_matrix(ConfusionMatrix cm)
{
    printf("\n%-40s%30s%10f\n", "CONFUSION MATRIX", "Threshold", cm.threshold);
    _print_thick_line();
    printf(
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n",
        "Total", cm.total, "CP", cm.P, "CN", cm.N, "Prior", cm.prior, "ACC", cm.ACC
    );
    _print_thin_line();
    printf(
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n"
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n",
        "PCP", cm.PP, "TP", cm.TP, "FP", cm.FP, "*PPV", cm.PPV, "FDR", cm.FDR,
        "PCN", cm.PN, "FN", cm.FN, "TN", cm.TN, "FOR", cm.FOR, "NPV", cm.NPV
    );
    _print_thin_line();
    printf(
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f\n"
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f    %3s: %.4f\n"
        "%13s  |  %28s  |  %5s: %.4f    %3s: %.4f\n",
        "", "**TPR", cm.TPR, "FPR", cm.FPR, "LR+", cm.LRP,
        "", "FNR", cm.FNR, "TNR", cm.TNR, "LR-", cm.LRN, "F1", cm.F1,
        "", "", "DOR", cm.DOR, "MCC", cm.MCC
    );
    _print_thick_line();
    printf("%80s\n", "* Precision   ** Recall");

    printf("\nKEY VALUES\n");
    _print_key_values_header();
    _print_key_values(cm);
    _print_thick_line();
}

void _print_key_values_header()
{
    _print_thick_line();
    printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n",
        "Threshold", "Accuracy", "Precision", "Recall", "Fall-out", "F1 score", "MCC"
    );
    _print_thin_line();
}

void _print_key_values(ConfusionMatrix cm)
{
    printf("%-12.6f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f\n",
        cm.threshold, cm.ACC, cm.PPV, cm.TPR, cm.FPR, cm.F1, cm.MCC
    );
}

void _print_headline_features(uint8_t feature_vector, int count) {
    int i;

    for (i = count - 1; i >= 0; i--) {
        printf("%u", feature_vector % 2 == 1 ? 1 : 0);
        feature_vector >>= 1;
    }
}

void _print_thin_line()
{
    printf("--------------------------------------------------------------------------------\n");
}

void _print_thick_line()
{
    printf("================================================================================\n");
}
