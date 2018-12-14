#include "classifier.h"

void classify_array( Headline *headlines, uint16_t headline_count, Feature *features, double threshold ) {
    uint16_t i;
    for ( i = 0; i < headline_count; i++ ) {
        classify( headlines + i, features, threshold );
    }
}

int8_t classify( Headline *headline, Feature *features, double threshold ) {
    double prob;

    headline->feature_vector = _get_feature_vector(headline, features);
    prob = _calculate_cb_prob(headline->feature_vector, features);
    headline->classified_clickbait = prob >= threshold ? 1 : 0;

    return headline->classified_clickbait;
}

Feature* calculate_feature_array( Headline* headlines, uint16_t headline_count ) {
    uint16_t i;
    Feature *features;
    
    features = get_features();

    for ( i = 0; i < headline_count; i++ ) {
        _add_feature_count( headlines[i], features );
    }

    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        /* p(F) = |F|/|A| */
        features[i].prob_feature = (double)features[i].feature_count / headline_count;

        if (features[i].feature_count > 0) {
            /* p(CB|F) = |CB & F| / |F| */
            features[i].prob_cb_given_feature = (double)features[i].feature_cb_count / features[i].feature_count;
        }
        else {
            features[i].prob_cb_given_feature = 0.5;
        }
    }

    return features;
}

double calculate_threshold(Headline* headlines, uint16_t headline_count, Feature *features) {
    int i, count_cb = 0, count_ncb = 0;
    double threshold = 0.5, prob, *cb_probs, *ncb_probs;

    cb_probs = malloc(headline_count * 0.5 * sizeof(double));
    ncb_probs = malloc(headline_count * 0.5 * sizeof(double));

    if (cb_probs == NULL || ncb_probs == NULL)
        exit(EXIT_FAILURE);

    for (i = 0; i < headline_count; i++) {
        prob = _calculate_cb_prob(_get_feature_vector(headlines + i, features), features);

        if (headlines[i].labeled_clickbait)
            cb_probs[count_cb++] = prob;
        else
            ncb_probs[count_ncb++] = prob;
    }

    double_array_sort(ncb_probs, count_ncb);
    double_array_sort(cb_probs, count_cb);

    /* set threshold to the median average */
    threshold = (double_array_median(ncb_probs, count_ncb) + double_array_median(cb_probs, count_cb)) / 2;

    return threshold;
}

uint8_t _get_feature_vector( Headline *headline, Feature *features ) {
    uint8_t i, feature_vector;

    for ( i = 0; i < FEATURE_COUNT; i++ ) {
        feature_vector <<= 1;
        feature_vector += features[i].has_feature(headline->title);
    }

    return feature_vector;
}

double _calculate_cb_prob( uint8_t feature_vector, Feature *features ) {
    double prob = 1;
    int8_t i;

    for ( i = FEATURE_COUNT - 1; i >= 0; i-- ) {
        double pcbf = features[i].prob_cb_given_feature;
        double pf = features[i].prob_feature;

        /* if headline has feature */
        if ( feature_vector % 2 == 1 ) { /* Only check first bit */
            /* multiply with p(CB|F) */
            prob *= pcbf;
        }
        else {
            /* multiply with p(CB|!F) */
            prob *= ( 0.5 - pcbf * pf ) / ( 1 - pf );
        }

        feature_vector >>= 1;
    }

    return prob;
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