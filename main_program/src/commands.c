#include "commands.h"

Commands import_commands()
{
    int i = 0;
    Commands exported;

    if ((exported.commands = (Command*) calloc(5, sizeof(Command))) == NULL) fatal_error();

    _export_command("exit", c_exit, i++, exported);
    _export_command("help", c_help, i++, exported);
    _export_command("train", c_train, i++, exported);
    _export_command("test", c_test, i++, exported);
    _export_command("threshold", c_threshold, i++, exported);

    exported.count = i;

    return exported;
}




int c_exit(const char **argv)
{
    return 0;
}




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
    _print_thick_line();

    return 1;
}




int c_train(const char **argv)
{
    char filename[MAX_FN_LEN];
    DataSet training_set;
    FeatureSet trained_features;

    _config_fallback(argv, 2, "TRAINING_DATASET_PATH", filename);

    if (strlen(filename) == 0) {
        error("Training dataset path missing");
        return 0;
    }

    training_set = import_headline_csv(filename);
    printf("Imported \"%s\", with %d records.\n", filename, training_set.count);

    trained_features = train_features(training_set);

    printf("Features successfully trained.\n");
    if (_flag_set(argv, "--print") != -1) _print_trained_features(trained_features);

    return 1;
}




int c_test(const char **argv)
{
    int i;
    double auc;
    char filename[MAX_FN_LEN], save_path[MAX_FN_LEN];
    DataSet test_set;
    FeatureSet trained_features;
    EvaluationSet evaluation;

    _config_fallback(argv, 2, "TEST_DATASET_PATH", filename);

    if (strlen(filename) == 0) {
        error("Test dataset path missing");
        return 0;
    }

    test_set = import_headline_csv(filename);
    printf("Imported \"%s\", with %d records.\n", filename, test_set.count);

    if (!load_trained_features(&trained_features)) {
        error("Features not trained");
        return 0;
    }

    score_dataset(test_set, trained_features);

    evaluation = evaluate_classifier(test_set);
    if (_flag_set(argv, "--print") != -1) _print_evaluation(evaluation);

    auc = calculate_AUC(evaluation);
    printf("ROC-AUC = %f\n", auc);

    if ((i = _flag_set(argv, "--save")) != -1) {
        _config_fallback(argv, (i+1), "TEST_SAVE_PATH", save_path);
        write_evaluation_file(evaluation, strlen(save_path) != 0 ? save_path : "test_results.csv");
    }

    return 1;
}




int c_threshold(const char **argv)
{
    double threshold;
    char threshold_str[MAX_FN_LEN];
    DataSet dataset;
    FeatureSet trained_features;
    ConfusionMatrix cm;

    if (_flag_set(argv, "-calc") != -1 || _flag_set(argv, "--print") != -1) {
        if (!_load_dataset(argv, &dataset, 0)) {
            return 0;
        }
        if (!load_trained_features(&trained_features)) {
            error("Features not trained");
            return 0;
        }
    }

    if (_flag_set(argv, "-calc") != -1) {
        threshold = calculate_threshold(dataset, trained_features);

        if (_flag_set(argv, "--save") != -1) {
            sprintf(threshold_str, "%f", threshold);
            save_config("THRESHOLD", threshold_str);
        }
        else {
            printf("Optimal threshold would be: %f\n", threshold);
        }
    }
    else if ((threshold = _get_threshold(argv, 2)) == -1) {
        return 0;
    }
    
    if (_flag_set(argv, "-calc") == -1 || _flag_set(argv, "--save") != -1)
        printf("Threshold is set to: %f\n", threshold);

    if (_flag_set(argv, "--print") != -1) {
        classify_dataset(dataset, trained_features, threshold);

        cm = evaluate_classification(dataset, threshold);

        _print_confusion_matrix(cm);
    }

    return 1;
}




void _export_command(char name[], int (*func)(const char**), int i, Commands exported) {
    exported.commands[i].func = func;
    strncpy(exported.commands[i].name, name, COMMAND_NAME_LEN);
}




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




int _flag_set(const char **argv, char *flag)
{
    int i;

    for (i = 2; i < 10 && argv[i] != NULL; i++) {
        if (strcmp(argv[i], flag) == 0) return i;
    }

    return -1;
}




int _is_value(const char *str)
{
    return str != NULL && str[0] != '-';
}




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

    if (_flag_set(argv, "--save") != -1 || _flag_set(argv, "--print") == -1)
        save_config("THRESHOLD", threshold_str);

    sscanf(threshold_str, "%lf", &threshold);

    return threshold;
}




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

void _print_thin_line()
{
    printf("--------------------------------------------------------------------------------\n");
}

void _print_thick_line()
{
    printf("================================================================================\n");
}
