#include "classifier.h"

void classify_array( Headline *headlines, uint16_t headline_count, Feature *features ) {
    uint16_t i;
    for ( i = 0; i < headline_count; i++ ) {
        printf("%d: ", i);
        classify( headlines + i, features );
    }
}

int classify( Headline *headline, Feature *features ) {
    double prob_cb;

    _set_feature_vector( headline, features );
    printf("feature vector %d, ", headline->feature_vector);

    prob_cb = _calculate_cb_prob( *headline, features );
    printf("cb prob: %lf\n", prob_cb);

    headline->classified_clickbait = ( prob_cb >= PROB_THRESHOLD )? 1: 0;
    return headline->classified_clickbait;
}

Feature *calculate_feature_array( Headline* headlines, uint16_t headline_count ) {
    uint16_t i;

    Feature *features = (Feature*)calloc(FEATURE_COUNT, sizeof(Feature));
    if ( features == NULL )
        exit(EXIT_FAILURE);

    features[0].has_feature = &longest_word;
    features[1].has_feature = &special_words;
    features[2].has_feature = &total_length;
    features[3].has_feature = &has_cb_punctuation;
    features[4].has_feature = &has_cb_hastag;
    features[5].has_feature = &begins_with_number;

    for ( i = 0; i < headline_count; i++ ) {
        _add_feature_count( headlines[i], features );
    }

    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        /* p(F) = |F|/|A| */
        features[i].prob_feature = (double)features[i].feature_count / headline_count;

        if (features[i].feature_count > 0) {
            /* p(CB|F) = |CB & F| / |F| */
            features[i].prob_cb_given_feature = (double)features[i].feature_cb_count / features[i].feature_count;
        } else {
            features[i].prob_cb_given_feature = 1;
        }
    }

    return features;
}

void _set_feature_vector( Headline* headline, Feature* features ) {
    uint8_t i = 0;
    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        headline->feature_vector <<= 1;
        headline->feature_vector += features[i].has_feature(headline->title);
    }
}

void _add_feature_count( Headline headline, Feature *features ) {
    uint8_t j;
    for ( j = 0; j < FEATURE_COUNT; j++ ) {
        if ( features[j].has_feature(headline.title) ) {
            features[j].feature_count++;

            if ( headline.labeled_clickbait ) {
                features[j].feature_cb_count++;
            }
        }
    }
}

double _calculate_cb_prob( Headline headline, Feature* features ) {
    double prob = 1;
    int8_t i;
    for ( i = FEATURE_COUNT - 1; i >= 0; i-- ) {
        double pcbf = features[i].prob_cb_given_feature;
        double pf = features[i].prob_feature;

        if ( headline.feature_vector % 2 == 1 ) { /* Only check first bit */
            prob *= pcbf;
        } else {
            prob *= ( 0.5 - pcbf * pf ) / ( 1 - pf );
        }

        headline.feature_vector >>= 1;
    }
    return prob;
}
