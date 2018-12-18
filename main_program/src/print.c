#include "print.h"

/* internal functions */
static void _print_feature(Feature);
static void _print_key_values_header(void);
static void _print_key_values(ConfusionMatrix);
static void _print_thin_line(void);
static void _print_thick_line(void);


/**
 * Prints a list of commands
 */

void print_command_list(void)
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
        "\tSubr.:\t--calc:           calculate optimal threshold\n"
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
}


/**
 * Outputs a list of trained features
 */

void print_trained_features(FeatureSet featureset)
{
    uint8_t i;
    printf("\n%-23s %10s %10s %10s\n", "Feature", "p(CB|F)", "p(CB|!F)", "p(F)");

    for ( i = 0; i < featureset.count; i++ ) {
        _print_feature(featureset.features[i]);
    }
}


/**
 * Prints a list of key values from an evaluation set (array of confusion matrixes)
 */

void print_evaluation(EvaluationSet evaluation)
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

void print_confusion_matrix(ConfusionMatrix cm)
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


/**
 * Prints the binary value of a feature vector
 */

void print_headline_features(uint8_t feature_vector, int count) {
    int i;

    for (i = count - 1; i >= 0; i--) {
        printf("%u", feature_vector % 2 == 1 ? 1 : 0);
        feature_vector >>= 1;
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




void _print_key_values_header(void)
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




void _print_thin_line(void)
{
    printf("--------------------------------------------------------------------------------\n");
}

void _print_thick_line(void)
{
    printf("================================================================================\n");
}
