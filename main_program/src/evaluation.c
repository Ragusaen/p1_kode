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

ROC_point *calculate_ROC(Headline *headlines, int count) {

    ROC_point *data_points = (ROC_point*)malloc( ROC_POINTS * sizeof(ROC_point) );
    if ( data_points == NULL ) {
        printf("Error allocating memory: data_points");
        exit(EXIT_FAILURE);
    }

    return data_points;
}

ConfusionMatrix calc_confusion_matrix(Headline *data_in, int data_amount) {
    int i;
    uint8_t c, l;
    double mcc_denom;
    ConfusionMatrix cm;

    /* reset counters */
    cm.P = 0; cm.N = 0; cm.TP = 0; cm.FP = 0; cm.FN = 0; cm.TN = 0;

    for (i = 0; i < data_amount; i++) {
        l = data_in[i].labeled_clickbait;
        c = data_in[i].classified_clickbait;

        if ( l == 1 ) {
            cm.P++;
            if ( c == 1 ) cm.TP++; else cm.FN++;
        }
        else {
            cm.N++;
            if ( c == 1 ) cm.FP++; else cm.TN++;
        }
    }

    /* total population */
    cm.total = data_amount;

    /* prevalence, prior */
    cm.prior = (double) cm.P / cm.total;

    /* predicted condition positive */
    cm.PP = cm.TP + cm.FP;
    /* predicted condition negative */
    cm.PN = cm.FN + cm.TN;

    /* sensitivity, recall, hit rate, or true positive rate (TPR) */
    cm.TPR = (double) cm.TP / cm.P;
    /* specificity, selectivity or true negative rate (TNR) */
    cm.TNR = (double) cm.TN / cm.N;
    /* precision or positive predictive value (PPV) */
    cm.PPV = (double) cm.TP / cm.PP;
    /* negative predictive value (NPV) */
    cm.NPV = (double) cm.TN / cm.PN;
    /* miss rate or false negative rate (FNR) */
    cm.FNR = (double) cm.FN / cm.P;
    /* fall-out or false positive rate (FPR) */
    cm.FPR = (double) cm.FP / cm.N;
    /* false discovery rate (FDR) */
    cm.FDR = (double) cm.FP / (cm.FP + cm.TP);
    /* false omission rate (FOR) */
    cm.FOR = (double) cm.FN / (cm.FN + cm.TN);

    /* accuracy (ACC) */
    cm.ACC = (double) (cm.TP + cm.TN) / cm.total;

    /* positive likelihood ratio (LR+) */
    cm.LRP = cm.TPR / cm.FPR;
    /* negative likelihood ratio (LR-) */
    cm.LRN = cm.FNR / cm.TNR;
    /* diagnostic odds ratio (DOR) */
    cm.DOR = cm.LRP / cm.LRN;

    /* F1 score */
    cm.F1 = 2 / (1 / cm.TPR + 1 / cm.PPV);
    /* Matthews correlation coefficient (MCC) */
    mcc_denom = (double) sqrt(cm.PP * cm.P * cm.N * cm.PN);
    cm.MCC = (double) (cm.TP * cm.TN - cm.FP * cm.FN) / (mcc_denom != 0 ? mcc_denom : 1);
    /* Informedness or Bookmaker Informedness (BM) */
    cm.BM = cm.TPR + cm.TNR - 1;
    /* Markedness (MK) */
    cm.MK = cm.PPV + cm.NPV - 1;

    return cm;
}