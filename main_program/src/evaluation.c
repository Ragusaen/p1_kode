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

void print_confusion_matrix(Headline *data_in, int data_amount) {
    ResultCounter rc = _count_results(data_in, data_amount);

    printf("\n"
        "Total %-6.0f   CP %-6.0f   CN %-6.0f  Prevalence %-6.4f  ACC %-6.4f\n"
        "  PCP %-6.0f   TP %-6.0f   FP %-6.0f   Precision %-6.4f  FDR %-6.4f\n"
        "  PCN %-6.0f   FN %-6.0f   TN %-6.0f         FOR %-6.4f  NPV %-6.4f\n"
        "              TPR %-6.4f  FPR %-6.4f         LR+ %-6.4f  DOR %-6.4f\n"
        "              FNR %-6.4f  TNR %-6.4f         LR- %-6.4f   F1 %-6.4f\n"
        "                                                         MCC %-6.4f\n",
        rc.P + rc.N, rc.P, rc.N, rc.P / (rc.P + rc.N), rc.ACC,
        rc.TP + rc.FP, rc.TP, rc.FP, rc.PPV, rc.FDR,
        rc.FN + rc.TN, rc.FN, rc.TN, rc.FOR, rc.NPV,
        rc.TPR, rc.FPR, rc.TPR / rc.FPR, (rc.TPR / rc.FPR) / (rc.FNR / rc.TNR),
        rc.FNR, rc.TNR, rc.FNR / rc.TNR, rc.F1,
        rc.MCC
    );
}

ResultCounter _count_results(Headline *data_in, int data_amount) {
    int i;
    double denom;
    ResultCounter rc;

    for (i = 0; i < data_amount; i++) {
        if ( data_in[i].labeled_clickbait ) {
            rc.P++;

            if ( data_in[i].classified_clickbait )
                rc.TP++;
            else
                rc.FN++;
        }
        else {
            rc.N++;

            if ( data_in[i].classified_clickbait )
                rc.FP++;
            else
                rc.TN++;
        }
    }

    /* sensitivity, recall, hit rate, or true positive rate (TPR) */
    rc.TPR = rc.TP / rc.P;

    /* specificity, selectivity or true negative rate (TNR) */
    rc.TNR = rc.TN / rc.N;

    /* precision or positive predictive value (PPV) */
    rc.PPV = rc.TP / (rc.TP + rc.FP);

    /* negative predictive value (NPV) */
    rc.NPV = rc.TN / (rc.TN + rc.FN);

    /* miss rate or false negative rate (FNR) */
    rc.FNR = rc.FN / rc.P;

    /* fall-out or false positive rate (FPR) */
    rc.FPR = rc.FP / rc.N;

    /* false discovery rate (FDR) */
    rc.FDR = rc.FP / (rc.FP + rc.TP);

    /* false omission rate (FOR) */
    rc.FOR = rc.FN / (rc.FN + rc.TN);

    /* accuracy (ACC) */
    rc.ACC = (rc.TP + rc.TN) / (rc.P + rc.N);

    /* F1 score */
    rc.F1 = 2 * rc.PPV * rc.TPR / (rc.PPV + rc.TPR);

    /* Matthews correlation coefficient (MCC) */
    denom = sqrt((rc.TP + rc.FP) * (rc.TP + rc.FN) * (rc.TN + rc.FP) * (rc.TN + rc.FN));
    rc.MCC = (rc.TP * rc.TN - rc.FP * rc.FN) / (denom != 0 ? denom : 1);

    /* Informedness or Bookmaker Informedness (BM) */
    rc.BM = rc.TPR + rc.TNR - 1;

    /* Markedness (MK) */
    rc.MK = rc.PPV + rc.NPV - 1;

    return rc;
}