/*
    Automatic detection of clickbait


*/

/* This controls wether or not the program is in debugging mode for conditional compilation */
#define DEBUG_MODE 1

/* Include libraries */
#include <stdlib.h>
#include <stdio.h>

/* Include headers */
#include "classifier.h"
#include "headline.h"
#include "features.h"
#include "recall_precision.h"

void print_classification( Headline *test_data, int test_count );

/* Entrypoint for the program */
int main( int argc, const char* argv[] ) {
    Headline *training_data;
    int training_count;
    Headline *test_data;
    int test_count;

    Feature* feature_probabilities;
    import_csv( &training_data, &training_count, "res/training_data.csv" );
    printf("Imported training data, with %d points\n", training_count);

    feature_probabilities = calculate_feature_array( training_data, training_count );
    printf("Calculated feature array\n");

    import_csv( &test_data, &test_count, "res/test_data.csv");
    printf("Imported test data, with %d points\n", test_count);

    classify_array( test_data, test_count, feature_probabilities );
    printf("Classified array\n");

    print_classification( test_data, test_count );
    printf("\nProgram finished, exiting...\n");

    return EXIT_SUCCESS;
}

void print_classification( Headline *test_data, int test_count ) {
    int i;
    for ( i = 0; i < test_count; i++ ) {
        int k = ( test_data[i].labeled_clickbait == test_data[i].classified_clickbait );
        int l = test_data[i].classified_clickbait;
        printf("%s: was %s classified as %s clickbait.\n", test_data[i].title, k?"correctly":"falsely", l?"":"not");
    }
}
