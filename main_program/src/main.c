/**
 *  Automatic detection of clickbait
 *
 *
 **/

/* This controls wether or not the program is in debugging mode for conditional compilation */
#define DEBUG_MODE 1

#define PROBABILITY_THRESHOLD 0.000409

/* Include libraries */
#include <stdlib.h>
#include <stdio.h>

/* Include headers */
#include "classifier.h"
#include "headline.h"
#include "features.h"
#include "train.h"
#include "evaluation.h"

void print_trained_features(FeatureSet featureset);
void print_feature(Feature feature);

void print_evaluation(EvaluationSet evaluation);
void print_confusion_matrix(ConfusionMatrix cm);

void print_key_values_header();
void print_key_values(ConfusionMatrix cm);

void print_thin_line();
void print_thick_line();

/* Entrypoint for the program */
int main(int argc, const char* argv[])
{
    double threshold, auc;
    DataSet training_set, test_set;
    FeatureSet trained_features;
    ConfusionMatrix confusion_matrix;
    EvaluationSet evaluation;

    training_set = import_headline_csv("res/training.csv");
    printf("Imported training data, with %d points\n", training_set.count);

    trained_features = train_features(training_set);
    printf("\nTrained features\n");
    print_trained_features(trained_features);

    threshold = calculate_threshold(training_set, trained_features);
    printf("\nCalculated median threshold: %f\n", threshold);

    test_set = import_headline_csv("res/test.csv");
    printf("\nImported test data, with %d points.\n", test_set.count);

    classify_dataset( test_set, trained_features, threshold );
    printf("Test data successfully classified.\n");

    confusion_matrix = evaluate_classification(test_set, threshold);
    print_confusion_matrix(confusion_matrix);

    evaluation = evaluate_classifier(test_set);
    
    /*print_evaluation(evaluation);*/

    auc = calculate_AUC(evaluation);
    printf("\nROC-AUC = %f\n", auc);

    write_evaluation_file(evaluation, "evaluation/evaluation.csv");
    printf("\nEvaluation file exported.\n");

    printf("\n\nProgram finished, exiting...\n");

    return EXIT_SUCCESS;
}

void print_trained_features(FeatureSet featureset)
{
    uint8_t i;
    printf("\n%-23s %10s %10s %10s\n", "Feature", "p(CB|F)", "p(CB|!F)", "p(F)");

    for ( i = 0; i < featureset.count; i++ ) {
        print_feature(featureset.features[i]);
    }
}

void print_feature(Feature feature)
{
    printf("%-23s %10.4f %10.4f %10.4f\n",
        feature.name,
        feature.prob_cb_given_feature,
        ( 0.5 - feature.prob_cb_given_feature * feature.prob_feature ) / ( 1 - feature.prob_feature ),
        feature.prob_feature
    );
}


void print_evaluation(EvaluationSet evaluation)
{
    int i;

    printf("\nCLASSIFIER EVALUATION\n");
    print_key_values_header();

    for (i = 0; i < evaluation.count; i++) {
        print_key_values(evaluation.data[i]);
    }

    print_thick_line();
}

void print_confusion_matrix(ConfusionMatrix cm)
{
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

void print_key_values_header()
{
    print_thick_line();
    printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n",
        "Threshold", "Accuracy", "Precision", "Recall", "Fall-out", "F1 score", "MCC"
    );
    print_thin_line();
}

void print_key_values(ConfusionMatrix cm)
{
    printf("%-12.6f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f%-12.4f\n",
        cm.threshold, cm.ACC, cm.PPV, cm.TPR, cm.FPR, cm.F1, cm.MCC
    );
}

void print_thin_line()
{
    printf("--------------------------------------------------------------------------------\n");
}

void print_thick_line()
{
    printf("================================================================================\n");
}