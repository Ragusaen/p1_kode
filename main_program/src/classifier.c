#include "classifier.h"

void classify_dataset(DataSet set, FeatureSet featureset, double threshold ) {
    uint16_t i;
    
    for ( i = 0; i < set.count; i++ ) {
        classify( set.data + i, featureset, threshold );
    }
}

int8_t classify(Headline *headline, FeatureSet featureset, double threshold) {
    headline->feature_vector = _get_feature_vector(headline, featureset);
    headline->prob_cb = _calculate_cb_prob(headline->feature_vector, featureset);
    headline->classified_clickbait = threshold <= headline->prob_cb;

    return headline->classified_clickbait;
}

double calculate_threshold(DataSet set, FeatureSet featureset) {
    int i, count_cb = 0, count_ncb = 0;
    double threshold = 0.5, prob, *cb_probs, *ncb_probs;

    cb_probs = malloc(set.count * 0.5 * sizeof(double));
    ncb_probs = malloc(set.count * 0.5 * sizeof(double));

    if (cb_probs == NULL || ncb_probs == NULL)
        exit(EXIT_FAILURE);

    for (i = 0; i < set.count; i++) {
        prob = _calculate_cb_prob(_get_feature_vector(set.data + i, featureset), featureset);

        if (set.data[i].labeled_clickbait)
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

uint8_t _get_feature_vector(Headline *headline, FeatureSet featureset) {
    uint8_t i, feature_vector;

    for ( i = 0; i < featureset.count; i++ ) {
        feature_vector <<= 1;
        feature_vector += featureset.features[i].has_feature(headline->title);
    }

    return feature_vector;
}

double _calculate_cb_prob(uint8_t feature_vector, FeatureSet featureset) {
    double prob = 1;
    int8_t i;

    for ( i = featureset.count - 1; i >= 0; i-- ) {
        double pcbf = featureset.features[i].prob_cb_given_feature;
        double pf = featureset.features[i].prob_feature;

        /* if headline has feature */
        if ( feature_vector % 2 == 1 ) { /* Only check first bit */
            /* multiply with p(CB|F) */
            prob *= pcbf;
        }
        else {
            /* multiply with p(CB|!F) */
            prob *= _prob_given_not_feature( pcbf, pf );
        }

        feature_vector >>= 1;
    }

    return prob;
}

double _prob_given_not_feature( double pcbf, double pf ) {
    return ( 0.5 - pcbf * pf ) / ( 1 - pf );
}
