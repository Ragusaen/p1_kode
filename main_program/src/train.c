#include "train.h"

FeatureSet train_features(DataSet set) {
    uint16_t i;
    FeatureSet featureset = import_features();

    for ( i = 0; i < set.count; i++ ) {
        _add_feature_count(set.data[i], featureset);
    }

    for ( i = 0; i < featureset.count; i++ ) {
        /* p(F) = |F|/|A| */
        featureset.features[i].prob_feature = (double)featureset.features[i].feature_count / set.count;

        if (featureset.features[i].feature_count > 0) {
            /* p(CB|F) = |CB & F| / |F| */
            featureset.features[i].prob_cb_given_feature = (double)featureset.features[i].feature_cb_count / featureset.features[i].feature_count;
        }
        else {
            featureset.features[i].prob_cb_given_feature = 0.5;
        }
    }

    return featureset;
}

void _add_feature_count(Headline headline, FeatureSet featureset) {
    uint8_t j;
    for ( j = 0; j < featureset.count; j++ ) {
        if (featureset.features[j].has_feature(headline.title)) {
            featureset.features[j].feature_count++;

            if (headline.labeled_clickbait)
                featureset.features[j].feature_cb_count++;
        }
    }
}