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
void print_confusion_matrix(ConfusionMatrix cm);

/* Entrypoint for the program */
int main( int argc, const char* argv[] ) {
    Headline *training_data;
    int training_count;
    Headline *test_data;
    int test_count;
    double threshold;
    Feature* feature_probabilities;
    ConfusionMatrix confusion_matrix;

    import_csv( &training_data, &training_count, "res/training.csv" );
    printf("Imported training data, with %d points\n", training_count);

    feature_probabilities = calculate_feature_array( training_data, training_count );
    printf("\nCalculated feature array\n");
    print_feature_array( feature_probabilities );

    threshold = calculate_threshold(training_data, training_count, feature_probabilities);
    printf("\nThreshold = %f\n", threshold);

    import_csv( &test_data, &test_count, "res/test.csv");
    printf("\nImported test data, with %d points.\n", test_count);

    classify_array( test_data, test_count, feature_probabilities, threshold );
    printf("Test data successfully classified.\n");

    /*print_classification( test_data, test_count );*/

    confusion_matrix = calc_confusion_matrix(test_data, test_count);
    print_confusion_matrix(confusion_matrix);

    printf("\nProgram finished, exiting...\n");

    return EXIT_SUCCESS;
}

void print_classification( Headline *test_data, int test_count ) {
    int i;
    for ( i = 0; i < test_count; i++ ) {
        int k = ( test_data[i].labeled_clickbait == test_data[i].classified_clickbait );
        int l = test_data[i].classified_clickbait;
        printf("%s classified as %s\t\"%s\"\n",
            k ? "[correctly]" : " [falsely] ",
            l ? "[clickbait]    " : "[non-clickbait]",
            test_data[i].title
        );
    }
}

void print_feature_array( Feature *features ) {
    int i;
    char* feature_names[8] = {
        "has_no_long_word",
        "has_special_words",
        "is_short",
        "has_special_punctuation",
        "has_pronouns",
        "has_number",
        "has_low_average_word_length",
        "has_stop_words"
    };

    printf("\n%-30s %10s %10s %10s\n", "Feature", "p(CB|F)", "p(CB|!F)", "p(F)");

    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        printf("%-30s %10.4f %10.4f %10.4f\n",
            feature_names[i],
            features[i].prob_cb_given_feature,
            ( 0.5 - features[i].prob_cb_given_feature * features[i].prob_feature ) / ( 1 - features[i].prob_feature ),
            features[i].prob_feature
        );
    }

    printf("\n");
}

void print_headline_features(uint8_t feature_vector) {
    int i;

    for (i = FEATURE_COUNT - 1; i >= 0; i--) {
        printf("%u", feature_vector % 2 == 1 ? 1 : 0);
        feature_vector >>= 1;
    }
}

void print_confusion_matrix(ConfusionMatrix cm) {
    printf(
        "\nCONFUSION MATRIX\n"
        "=============================================================================\n"
    );
    printf(
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f  %3s: %.4f\n"
        "-----------------------------------------------------------------------------\n"
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f  %3s: %.4f\n"
        "%5s: %-6d  |  %5s: %-6d  %5s: %-6d  |  %5s: %.4f  %3s: %.4f\n",
        "Total", cm.total, "CP", cm.P, "CN", cm.N, "Prior", cm.prior, "ACC", cm.ACC,
        "PCP", cm.PP, "TP", cm.TP, "FP", cm.FP, "*PPV", cm.PPV, "FDR", cm.FDR,
        "PCN", cm.PN, "FN", cm.FN, "TN", cm.TN, "FOR", cm.FOR, "NPV", cm.NPV
    );
    printf(
        "-----------------------------------------------------------------------------\n"
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f\n"
        "%13s  |  %5s: %.4f  %5s: %.4f  |  %5s: %.4f  %3s: %.4f\n"
        "%13s  |  %28s  |  %5s: %.4f  %3s: %.4f\n",
        "", "**TPR", cm.TPR, "FPR", cm.FPR, "LR+", cm.LRP,
        "", "FNR", cm.FNR, "TNR", cm.TNR, "LR-", cm.LRN, "F1", cm.F1,
        "", "", "DOR", cm.DOR, "MCC", cm.MCC
    );
    printf(
        "=============================================================================\n"
    );
    printf(
        "\n"
        "KEY VALUES:   %s = %.4f   %s = %.4f   %s = %.4f\n"
        "\n"
        "              %s = %.4f   %s = %.4f\n"
        "\n",
        "Accuracy", cm.ACC, "*Precision", cm.PPV, "**Recall", cm.TPR, "F1 score", cm.F1, "MCC normalized", (cm.MCC + 1) / 2
    );
}