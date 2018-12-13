#include "classifier.h"

void classify_array( Headline *headlines, uint16_t headline_count, Feature *features, double threshold ) {
    uint16_t i;
    for ( i = 0; i < headline_count; i++ ) {
        classify( headlines + i, features, threshold );
    }
}

int8_t classify( Headline *headline, Feature *features, double threshold ) {
    int8_t class_;

    _set_feature_vector(headline, features);

    class_ = _calculate_cb_prob(*headline, features) >= threshold ? 1 : 0;
    /*class_ = _bernoulli_nb(*headline, features);*/

    headline->classified_clickbait = class_;

    return headline->classified_clickbait;
}

Feature* calculate_feature_array( Headline* headlines, uint16_t headline_count ) {
    uint16_t i;

    Feature *features = (Feature*)calloc(FEATURE_COUNT, sizeof(Feature));
    if ( features == NULL )
        exit(EXIT_FAILURE);

    features[0].has_feature = &has_no_long_word;
    features[1].has_feature = &has_special_words;
    features[2].has_feature = &is_short;
    features[3].has_feature = &has_special_punctuation;
    features[4].has_feature = &has_pronouns;
    features[5].has_feature = &has_number;
    features[6].has_feature = &has_low_average_word_length;
    features[7].has_feature = &has_stop_words;

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
    DoubleArrayCalc cb_calc, ncb_calc;

    cb_probs = malloc(headline_count * 0.5 * sizeof(double));
    ncb_probs = malloc(headline_count * 0.5 * sizeof(double));

    if (cb_probs == NULL || ncb_probs == NULL)
        exit(EXIT_FAILURE);

    for (i = 0; i < headline_count; i++) {
        _set_feature_vector(&headlines[i], features);

        prob = _calculate_cb_prob(headlines[i], features);

        if (headlines[i].labeled_clickbait)
            cb_probs[count_cb++] = prob;
        else
            ncb_probs[count_ncb++] = prob;
    }

    ncb_calc = double_array_calc(ncb_probs, count_ncb);
    cb_calc = double_array_calc(cb_probs, count_cb);

    printf("TRAINING PROBABILITY SCORES\n");
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n",
        "", "Sum", "Average", "Minimum", "Lower", "Median", "Upper", "Maximum"
    );
    printf("%-15s%-12f%-12f%-12f%-12f%-12f%-12f%-12f\n",
        "NON-CLICKBAIT",
        ncb_calc.sum, ncb_calc.avg, ncb_calc.min, ncb_calc.lower, ncb_calc.median, ncb_calc.upper, ncb_calc.max
    );
    printf("%-15s%-12f%-12f%-12f%-12f%-12f%-12f%-12f\n",
        "CLICKBAIT",
        cb_calc.sum, cb_calc.avg, cb_calc.min, cb_calc.lower, cb_calc.median, cb_calc.upper, cb_calc.max
    );

    /* set threshold to the median average */
    threshold = (ncb_calc.median + cb_calc.median) / 2;

    return threshold;
}

void _set_feature_vector( Headline *headline, Feature *features ) {
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

double _calculate_cb_prob( Headline headline, Feature *features ) {
    double prob = 0.5;
    int8_t i;

    for ( i = FEATURE_COUNT - 1; i >= 0; i-- ) {
        double pcbf = features[i].prob_cb_given_feature;
        double pf = features[i].prob_feature;

        /* if headline has feature */
        if ( headline.feature_vector % 2 == 1 ) { /* Only check first bit */
            /* multiply with p(CB|F) */
            prob *= pcbf;
        }
        else {
            /* multiply with p(CB|!F) */
            prob *= ( 0.5 - pcbf * pf ) / ( 1 - pf );
        }

        headline.feature_vector >>= 1;
    }

    return prob;
}


double _bernoulli_nb( Headline headline, Feature *features ) {
    int8_t class_, i;
    double score0 = log(0.5),
           score1 = log(0.5),
           p_f, p_cb_f, p_cb_nf;

    for ( i = FEATURE_COUNT - 1; i >= 0; i-- ) {
        p_f = features[i].prob_feature;
        p_cb_f = features[i].prob_cb_given_feature;
        p_cb_nf = ( 0.5 - p_cb_f * p_f ) / ( 1 - p_f );

        /* if headline has feature */
        if ( headline.feature_vector % 2 == 1 ) {
            score0 += log(1 - p_cb_f);
            score1 += log(p_cb_f);
        }
        else {
            score0 += log(1 - p_cb_nf);
            score1 += log(p_cb_nf);
        }

        headline.feature_vector >>= 1;
    }

    class_ = score0 > score1 ? 0 : 1;

    /*printf("\n%s\nclass %d  label %d \t %5s %s \t s0 %-10f  s1 %-10f \n",
        headline.title,
        class_,
        headline.labeled_clickbait,
        class_ == headline.labeled_clickbait ? "true" : "false",
        class_ == 1 ? "positive" : "negative",
        score0,
        score1
    );*/

    return class_;
}