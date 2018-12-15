/**
 *  Automatic detection of clickbait
 *
 *
 **/

/* This controls wether or not the program is in debugging mode for conditional compilation */
#define DEBUG_MODE 1

#define PROBABILITY_THRESHOLD 0.003448

/* Include libraries */
#include <stdlib.h>
#include <stdio.h>

/* Include headers */
#include "classifier.h"
#include "headline.h"
#include "features.h"
#include "evaluation.h"

void print_classification( Headline *test_data, int test_count );
void print_feature_array( Feature *features );
void print_headline_features(uint8_t feature_vector);
void print_evaluation(EvaluationSet evaluation);
void print_confusion_matrix(ConfusionMatrix cm);
void print_key_values_header();
void print_key_values(ConfusionMatrix cm);
void print_thin_line();
void print_thick_line();

/* Entrypoint for the program */
int main( int argc, const char* argv[] ) {
    double threshold, auc;
    int training_count;
    int test_count;
    Headline *training_data;
    Headline *test_data;
    Feature* feature_probabilities;
    ConfusionMatrix confusion_matrix;
    EvaluationSet evaluation;

    import_csv( &training_data, &training_count, "res/training.csv" );
    printf("Imported training data, with %d points\n", training_count);

    feature_probabilities = calculate_feature_array( training_data, training_count );
    printf("\nCalculated feature array\n");
    print_feature_array( feature_probabilities );

    threshold = calculate_threshold(training_data, training_count, feature_probabilities);
    printf("\nCalculated median threshold: %f\n", threshold);

    import_csv( &test_data, &test_count, "res/test.csv");
    printf("\nImported test data, with %d points.\n", test_count);

    classify_array( test_data, test_count, feature_probabilities, threshold );
    printf("Test data successfully classified.\n");

    confusion_matrix = evaluate_classification(test_data, test_count, threshold);
    print_confusion_matrix(confusion_matrix);

    evaluation = evaluate_classifier(test_data, test_count);
    
    /*print_evaluation(evaluation);*/

    auc = calculate_AUC(evaluation);
    printf("\nROC-AUC = %f\n", auc);

    write_evaluation_file(evaluation, "evaluation/evaluation.csv");
    printf("\nEvaluation file exported.\n");

    printf("\n\nProgram finished, exiting...\n");

    return EXIT_SUCCESS;
}

void print_feature_array( Feature *features ) {
    uint8_t i;
    printf("\n%-23s %10s %10s %10s\n", "Feature", "p(CB|F)", "p(CB|!F)", "p(F)");

    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        printf("%-23s %10.4f %10.4f %10.4f\n",
            features[i].name,
            features[i].prob_cb_given_feature,
            ( 0.5 - features[i].prob_cb_given_feature * features[i].prob_feature ) / ( 1 - features[i].prob_feature ),
            features[i].prob_feature
        );
    }
}

void print_headline_features(uint8_t feature_vector) {
    int i;

    for (i = FEATURE_COUNT - 1; i >= 0; i--) {
        printf("%u", feature_vector % 2 == 1 ? 1 : 0);
        feature_vector >>= 1;
    }
}


void print_evaluation(EvaluationSet evaluation) {
    int i;

    printf("\nCLASSIFIER EVALUATION\n");
    print_key_values_header();

    for (i = 0; i < evaluation.count; i++) {
        print_key_values(evaluation.data[i]);
    }

    print_thick_line();
}

void print_confusion_matrix(ConfusionMatrix cm) {
    printf("\nCONFUSION MATRIX\n");
    print_thick_line();
    printf(
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n",
        "Total", cm.total, "CP", cm.P, "CN", cm.N, "Prior", cm.prior, "ACC", cm.ACC
    );
    print_thin_line();
    printf(
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n"
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f    %3s: %.4f\n",
        "PCP", cm.PP, "TP", cm.TP, "FP", cm.FP, "*PPV", cm.PPV, "FDR", cm.FDR,
        "PCN", cm.PN, "FN", cm.FN, "TN", cm.TN, "FOR", cm.FOR, "NPV", cm.NPV
    );
    print_thin_line();
    printf(
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f\n"
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f    %3s: %.4f\n"
        "%13s  |  %28s  |  %5s: %.4f    %3s: %.4f\n",
        "", "**TPR", cm.TPR, "FPR", cm.FPR, "LR+", cm.LRP,
        "", "FNR", cm.FNR, "TNR", cm.TNR, "LR-", cm.LRN, "F1", cm.F1,
        "", "", "DOR", cm.DOR, "MCC", cm.MCC
    );
    print_thick_line();
    printf("%80s\n", "* Precision   ** Recall");
    printf("\nKEY VALUES\n");
    print_key_values_header();
    print_key_values(cm);
    print_thick_line();
}

void print_key_values_header() {
    print_thick_line();
    printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n",
        "Threshold", "Accuracy", "Precision", "Recall", "Fall-out", "F1 score", "MCC"
    );
    print_thin_line();
}

void print_key_values(ConfusionMatrix cm) {
    printf("%-12.6f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f\n",
        cm.threshold, cm.ACC, cm.PPV, cm.TPR, cm.FPR, cm.F1, cm.MCC
    );
}

void print_thin_line() {
    printf("--------------------------------------------------------------------------------\n");
}

void print_thick_line() {
    printf("================================================================================\n");
}