#include "core/classifier.h"

/* internal functions */
static void _score_headline(Headline *, FeatureSet);
static uint8_t _get_feature_vector(char *, FeatureSet);
static double _calculate_prob_score(uint8_t, FeatureSet);
static double _prob_given_not_feature(double, double);


/**
 * Classify a dataset.
 * 
 * @param dataset       the dataset to classify
 * @param featureset    a trained set of features
 * @param threshold     the threshold to classify by
 */

void classifier_classify_dataset(DataSet dataset, FeatureSet featureset, double threshold)
{
    uint16_t i;

    for ( i = 0; i < dataset.count; i++ ) {
        classifier_classify_headline(dataset.data + i, featureset, threshold);
    }
}


/**
 * Classify a headline item.
 * 
 * @param headline      a pointer to the headline item
 * @param featureset    a trained set of features
 * @param threshold     the threshold to classify by
 */

int8_t classifier_classify_headline(Headline *headline, FeatureSet featureset, double threshold)
{
    /* set probability score */
    _score_headline(headline, featureset);

    /* classify headline */
    headline->classified_clickbait = threshold <= headline->prob_score ? 1 : 0;

    return headline->classified_clickbait;
}


/**
 * Add probability scores to a dataset.
 * 
 * @param dataset       the dataset to score
 * @param featureset    a trained set of features
 */

void classifier_score_dataset(DataSet dataset, FeatureSet featureset)
{
    uint16_t i;

    for ( i = 0; i < dataset.count; i++ ) {
        _score_headline(dataset.data + i, featureset);
    }
}


/**
 * Calculates an optimal threshold, the average of the two median scores of clickbait and non-clickbait.
 * 
 * @param dataset       the dataset to use for probability scores
 * @param featureset    a trained set of features
 */

double classifier_calculate_threshold(DataSet dataset, FeatureSet featureset)
{
    int i, count_cb = 0, count_ncb = 0;
    double threshold = 0.5, prob_score, *cb_probs, *ncb_probs;

    /* two arrays of doubles, half size of dataset (50-50 distribution required) */
    if ((cb_probs = malloc(dataset.count * 0.5 * sizeof(double))) == NULL ||
        (ncb_probs = malloc(dataset.count * 0.5 * sizeof(double))) == NULL) fatal_error();

    for (i = 0; i < dataset.count; i++) {
        /* get probability score */
        prob_score = _calculate_prob_score(
            _get_feature_vector(dataset.data[i].content, featureset),
            featureset
        );

        /* add probability to either clickbait or non-clickbait array */
        if (dataset.data[i].labeled_clickbait)
            cb_probs[count_cb++] = prob_score;
        else
            ncb_probs[count_ncb++] = prob_score;
    }

    /* sort both arrays (required by double_array_median) */
    double_array_sort(ncb_probs, count_ncb);
    double_array_sort(cb_probs, count_cb);

    /* set threshold to the median average */
    threshold = (double_array_median(ncb_probs, count_ncb) + double_array_median(cb_probs, count_cb)) / 2;

    return threshold;
}


/**
 * Set feature vector and probability score on headline.
 */

void _score_headline(Headline *headline, FeatureSet featureset)
{
    headline->feature_vector = _get_feature_vector(headline->content, featureset);
    headline->prob_score = _calculate_prob_score(headline->feature_vector, featureset);
}


/**
 * Create a feature vector based on the features detected in str.
 */

uint8_t _get_feature_vector(char str_in[], FeatureSet featureset)
{
    uint8_t i, feature_vector;

    for (i = 0; i < featureset.count; i++) {
        feature_vector <<= 1;
        feature_vector += featureset.features[i].has_feature(str_in);
    }

    return feature_vector;
}


/**
 * Calculate probability score from a feature vector.
 */

double _calculate_prob_score(uint8_t feature_vector, FeatureSet featureset)
{
    double prob = 1;
    int8_t i;

    for (i = featureset.count - 1; i >= 0; i--) {
        double pcbf = featureset.features[i].prob_cb_given_feature;
        double pf = featureset.features[i].prob_feature;

        /* if headline has feature */
        if (feature_vector % 2 == 1) { /* Only check first bit */
            /* multiply with p(CB|F) */
            prob *= pcbf;
        }
        else {
            /* multiply with p(CB|!F) */
            prob *= _prob_given_not_feature(pcbf, pf);
        }

        feature_vector >>= 1;
    }

    return prob;
}


/**
 * p(CB|!F)
 */

double _prob_given_not_feature(double pcbf, double pf) {
    return ( 0.5 - pcbf * pf ) / ( 1 - pf );
}
