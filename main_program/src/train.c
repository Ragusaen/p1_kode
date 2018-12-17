#include "train.h"

int load_trained_features(FeatureSet *featureset)
{
    int i;
    FILE *fp;
    FeatureSet temp;

    *featureset = import_features();
    if ((temp.features = (Feature*) calloc(featureset->count, sizeof(Feature))) == NULL) fatal_error();

    if ((fp = fopen(TRAINED_FEATURES_FN, "r")) == NULL) {
        error("Couldn't load \"" TRAINED_FEATURES_FN "\"");
        return 0;
    }

    for (i = 0; i < featureset->count; i++) {
        fread(temp.features + i, sizeof(Feature), 1, fp);
        _copy_feature(featureset->features + i, temp.features[i]);
    }

    fclose(fp);

    return 1;
}


/**
 * Trains the features, eg. calculates probabilities.
 * 
 * @param set   the training data set
 */

FeatureSet train_features(DataSet dataset)
{
    uint16_t i;
    FeatureSet featureset;
    
    /* import features */
    featureset = import_features();

    /* count number of features for each headline */
    for ( i = 0; i < dataset.count; i++ ) {
        _add_feature_count(dataset.data[i], featureset);
    }

    /* calculate probabilities for each feature */
    for ( i = 0; i < featureset.count; i++ ) {
        _calculate_feature_probabilities(featureset.features + i, dataset.count);
    }

    _save_trained_features(featureset);

    return featureset;
}

void _add_feature_count(Headline headline, FeatureSet featureset)
{
    uint8_t j;
    for ( j = 0; j < featureset.count; j++ ) {
        if (featureset.features[j].has_feature(headline.content)) {
            featureset.features[j].feature_count++;

            if (headline.labeled_clickbait)
                featureset.features[j].feature_cb_count++;
        }
    }
}

void _calculate_feature_probabilities(Feature *feature, int data_count)
{
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


int _save_trained_features(FeatureSet featureset)
{
    int i;
    FILE *fp;

    if ((fp = fopen(TRAINED_FEATURES_FN, "w")) == NULL) {
        error("Couldn't save \"" TRAINED_FEATURES_FN "\"");
        return 0;
    }

    for (i = 0; i < featureset.count; i++) {
        fwrite(featureset.features + i, sizeof(Feature), 1, fp);
    }

    fclose(fp);

    return 1;
}

void _copy_feature(Feature *target, Feature original)
{
    target->prob_feature = original.prob_feature;
    target->prob_cb_given_feature = original.prob_cb_given_feature;
    target->feature_count = original.feature_count;
    target->feature_cb_count = original.feature_cb_count;
}