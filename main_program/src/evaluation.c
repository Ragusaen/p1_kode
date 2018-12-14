#include "evaluation.h"

double recall(Headline *data_in, int data_amount) {
    int i,
        clickbait_tp = 0,
        clickbait_lp = 0;

    for (i = 0; i < data_amount; i++){
        /* count all headlines labeled as clickbait */
        if ( data_in[i].labeled_clickbait ) {
            clickbait_lp++;

            /* count all the true positives */
            if ( data_in[i].classified_clickbait )
                clickbait_tp++;
        }
    }

    return (double) clickbait_tp / clickbait_lp;
}

double precision(Headline *data_in, int data_amount) {
    int i,
        clickbait_cp = 0,
        clickbait_tp = 0;

    for (i = 0; i < data_amount; i++) {
        /* count all the positives */
        if ( data_in[i].classified_clickbait ) {
            clickbait_cp++;

            /* count all the true positives */
            if ( data_in[i].labeled_clickbait )
                clickbait_tp++;
        }
    }

    return (double) clickbait_tp / clickbait_cp;
}

double f_measure(Headline *data_in, int data_amount){

    double beta = 1;
    double precision_ = precision(data_in, data_amount);
    double recall_ = recall(data_in, data_amount);

    return (1 + beta * beta) * precision_ * recall_ / ((beta * precision_) * recall_);
}

double calculate_ROC_AUC(Headline *headlines, int headline_count ) {
    double threshold, min_thres = 1, max_thres = 0, delta_thres;
    double last_fpr, last_tpr, ROC_auc = 0;

    _get_min_max_probs( headlines, headline_count, &min_thres, &max_thres );

    delta_thres = (max_thres - min_thres) / ROC_POINTS;
    last_fpr = 0; last_tpr = 0;
    for ( threshold = max_thres + delta_thres; threshold >= min_thres - delta_thres; threshold -= delta_thres ) {
        int i, tp = 0, fp = 0, p = 0;
        double tpr, fpr;
        for ( i = 0; i < headline_count; i++ ) {
            int c_cb = headlines[i].prob_cb >= threshold;
            int l_cb  = headlines[i].labeled_clickbait;
            if ( l_cb ) {
                p++;
                if ( c_cb )
                    tp++;
            } else if ( c_cb )
                fp++;
        }
        tpr = (double) tp / p;
        fpr = (double) fp / ( headline_count - p );

        printf("%f, %f\n", fpr, tpr );

        /* Rectangle under + triangle over */
        ROC_auc += last_tpr * ( fpr - last_fpr ) + 0.5 * ( tpr - last_tpr ) * ( fpr - last_fpr );

        last_fpr = fpr; last_tpr = tpr;
    }

    return ROC_auc;
}

TFPNCounter count_results(Headline *data_in, int data_amount) {
    int i;
    uint8_t cls, lbl;
    TFPNCounter c;

    /* reset counters */
    c.TP = 0; c.FP = 0; c.FN = 0; c.TN = 0;

    for (i = 0; i < data_amount; i++) {
        lbl = data_in[i].labeled_clickbait;
        cls = data_in[i].classified_clickbait;

        if ( lbl == 1 ) {
            if ( cls == 1 ) c.TP++; else c.FN++;
        }
        else {
            if ( cls == 1 ) c.FP++; else c.TN++;
        }
    }

    c.P  = c.TP + c.FN;      /* condition positive */
    c.N  = c.FP + c.TN;      /* condition negative */
    c.PP = c.TP + c.FP;     /* predicted condition positive */
    c.PN = c.FN + c.TN;     /* predicted condition negative */
    c.total = c.P + c.N;    /* total population */

    return c;
}

ConfusionMatrix calc_confusion_matrix(Headline *data_in, int data_amount) {
    double mcc_denom;
    ConfusionMatrix cm;
    cm.c = count_results(data_in, data_amount);

    /* prevalence, prior */
    cm.prior = (double) cm.c.P / cm.c.total;

    /* sensitivity, recall, hit rate, or true positive rate (TPR) */
    cm.TPR = (double) cm.c.TP / cm.c.P;
    /* specificity, selectivity or true negative rate (TNR) */
    cm.TNR = (double) cm.c.TN / cm.c.N;
    /* precision or positive predictive value (PPV) */
    cm.PPV = (double) cm.c.TP / cm.c.PP;
    /* negative predictive value (NPV) */
    cm.NPV = (double) cm.c.TN / cm.c.PN;
    /* miss rate or false negative rate (FNR) */
    cm.FNR = (double) cm.c.FN / cm.c.P;
    /* fall-out or false positive rate (FPR) */
    cm.FPR = (double) cm.c.FP / cm.c.N;
    /* false discovery rate (FDR) */
    cm.FDR = (double) cm.c.FP / (cm.c.FP + cm.c.TP);
    /* false omission rate (FOR) */
    cm.FOR = (double) cm.c.FN / (cm.c.FN + cm.c.TN);

    /* accuracy (ACC) */
    cm.ACC = (double) (cm.c.TP + cm.c.TN) / cm.c.total;

    /* positive likelihood ratio (LR+) */
    cm.LRP = cm.TPR / cm.FPR;
    /* negative likelihood ratio (LR-) */
    cm.LRN = cm.FNR / cm.TNR;
    /* diagnostic odds ratio (DOR) */
    cm.DOR = cm.LRP / cm.LRN;

    /* F1 score */
    cm.F1 = 2 / (1 / cm.TPR + 1 / cm.PPV);

    /* Matthews correlation coefficient (MCC) */
    /* (TP * TN - FP * FN) / sqrt(PP * P * N * PN) */
    mcc_denom = (double) sqrt(cm.c.PP * cm.c.P * cm.c.N * cm.c.PN);
    cm.MCC = (double) (cm.c.TP * cm.c.TN - cm.c.FP * cm.c.FN) / (mcc_denom != 0 ? mcc_denom : 1);

    /* Informedness or Bookmaker Informedness (BM) */
    cm.BM = cm.TPR + cm.TNR - 1;
    /* Markedness (MK) */
    cm.MK = cm.PPV + cm.NPV - 1;

    return cm;
}

void _get_min_max_probs( Headline *headlines, int count, double *min, double *max ) {
    int i;
    for ( i = 0; i < count; i++ ) {
        if ( headlines[i].prob_cb < *min )
            *min = headlines[i].prob_cb;
        if ( headlines[i].prob_cb > *max )
            *max = headlines[i].prob_cb;
    }
}
