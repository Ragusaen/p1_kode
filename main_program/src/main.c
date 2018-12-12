/**
 *  Automatic detection of clickbait
 *
 *
 **/

/* This controls wether or not the program is in debugging mode for conditional compilation */
#define DEBUG_MODE 1
#define PROBABILITY_THRESHOLD 0.01

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

/* Entrypoint for the program */
int main( int argc, const char* argv[] ) {
    Headline *training_data;
    int training_count;
    Headline *test_data;
    int test_count;

    Feature* feature_probabilities;

    import_csv( &training_data, &training_count, "res/training.csv" );
    printf("Imported training data, with %d points\n", training_count);

    feature_probabilities = calculate_feature_array( training_data, training_count );
    printf("Calculated feature array\n");
    print_feature_array( feature_probabilities );

    import_csv( &test_data, &test_count, "res/test.csv");
    printf("Imported test data, with %d points\n", test_count);

    classify_array( test_data, test_count, feature_probabilities, PROBABILITY_THRESHOLD );
    printf("Classified array\n");

    /*print_classification( test_data, test_count );*/

    printf("\nPrecision = %f\t Recall = %f\n",
        precision(test_data, test_count),
        recall(test_data, test_count)
    );

    print_confusion_matrix(test_data, test_count);

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
