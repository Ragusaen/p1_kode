#include "train.h"

/**
 * Trains the features, eg. calculates probabilities.
 * 
 * @param set   the training data set
 */

FeatureSet train_features(DataSet set) {
    uint16_t i;
    FeatureSet featureset;
    
    /* import features */
    featureset = import_features();

    /* count number of features for each headline */
    for ( i = 0; i < set.count; i++ ) {
        _add_feature_count(set.data[i], featureset);
    }

    /* calculate probabilities for each feature */
    for ( i = 0; i < featureset.count; i++ ) {
        _calculate_feature_probabilities(featureset.features + i, set.count);
    }

    return featureset;
}

void _add_feature_count(Headline headline, FeatureSet featureset) {
    uint8_t j;
    for ( j = 0; j < featureset.count; j++ ) {
        if (featureset.features[j].has_feature(headline.content)) {
            featureset.features[j].feature_count++;

            if (headline.labeled_clickbait)
                featureset.features[j].feature_cb_count++;
        }
    }
}

void _calculate_feature_probabilities(Feature *feature, int data_count) {
    /* p(F) = |F|/|A| */
    feature->prob_feature = (double) feature->feature_count / data_count;

    if (feature->feature_count > 0) {
        /* p(CB|F) = |CB & F| / |F| */
        feature->prob_cb_given_feature = (double) feature->feature_cb_count / feature->feature_count;
    }
    else {
        feature->prob_cb_given_feature = 1;
    }
}