#include "evaluation.h"

ConfusionMatrix evaluate_classification(Headline *headlines, int headline_count) {
    ResultCounter result = _count_true_false_positives(headlines, headline_count);
    
    ConfusionMatrix cm = _calc_confusion_matrix(result.P, result.N, result.TP, result.FP);

    return cm;
}

ResultCounter _count_true_false_positives(Headline *headlines, int headline_count) {
    int i;
    uint8_t cls, lbl;
    ResultCounter c;

    /* reset counters */
    c.P = 0; c.N = 0; c.TP = 0; c.FP = 0;

    for (i = 0; i < headline_count; i++) {
        lbl = headlines[i].labeled_clickbait;
        cls = headlines[i].classified_clickbait;

        if (lbl == 1) {
            c.P++;
            if (cls == 1) c.TP++;
        }
        else {
            c.N++;
            if (cls == 1) c.FP++;
        }
    }

    return c;
}

ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP) {
    double mcc_denom;
    ConfusionMatrix cm;
    
    cm.P = P; cm.N = N; cm.TP = TP; cm.FP = FP;

    cm.FN = cm.P - cm.TP;    /* false negatives */
    cm.TN = cm.N - cm.FP;    /* true negatives */
    cm.PP = cm.TP + cm.FP;     /* predicted condition positive */
    cm.PN = cm.FN + cm.TN;     /* predicted condition negative */
    cm.total = cm.P + cm.N;    /* total population */

    /* prevalence, prior */
    cm.prior = (double) cm.P / cm.total;

    /* sensitivity, recall, hit rate, or true positive rate (TPR) */
    cm.TPR = (double) cm.TP / cm.P;
    /* fall-out or false positive rate (FPR) */
    cm.FPR = (double) cm.FP / cm.N;
    /* miss rate or false negative rate (FNR) */
    cm.FNR = 1 - cm.TPR;
    /* specificity, selectivity or true negative rate (TNR) */
    cm.TNR = 1 - cm.FPR;
    /* precision or positive predictive value (PPV) */
    cm.PPV = (double) cm.TP / cm.PP;
    /* negative predictive value (NPV) */
    cm.NPV = (double) cm.TN / cm.PN;
    /* false discovery rate (FDR) */
    cm.FDR = 1 - cm.PPV;
    /* false omission rate (FOR) */
    cm.FOR = 1 - cm.NPV;

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
    /* (TP * TN - FP * FN) / sqrt(PP * P * N * PN) */
    mcc_denom = (double) sqrt(cm.PP * cm.P * cm.N * cm.PN);
    cm.MCC = (double) (cm.TP * cm.TN - cm.FP * cm.FN) / (mcc_denom != 0 ? mcc_denom : 1);

    /* Informedness or Bookmaker Informedness (BM) */
    cm.BM = cm.TPR + cm.TNR - 1;
    /* Markedness (MK) */
    cm.MK = cm.PPV + cm.NPV - 1;

    return cm;
}

ROC_set calculate_ROC(Headline *headlines, int headline_count) {
    int i, P = 0, N = 0, TP = 0, FP = 0, ROC_index = 0, prob_count = 0;
    double prev_prob = 1;
    ROC_set set;

    qsort(headlines, headline_count, sizeof(Headline), _compare_probabilities);

    for (i = 0; i < headline_count; i++) {
        if (headlines[i].labeled_clickbait) P++; else N++;
        if (headlines[i].prob_cb != prev_prob) {
            prev_prob = headlines[i].prob_cb;
            prob_count++;
        }
    }
    
    prob_count++;

    set.points = (ROC_point*) calloc(prob_count, sizeof(ROC_point));
    set.count = prob_count;

    prev_prob = 1;

    for (i = 0; i < headline_count; i++) {
        if (headlines[i].prob_cb != prev_prob) {
            set.points[ROC_index].TPR = (double) TP / P;
            set.points[ROC_index].FPR = (double) FP / N;
            set.points[ROC_index].threshold = prev_prob;
            prev_prob = headlines[i].prob_cb;
            ROC_index++;
        }

        if (headlines[i].labeled_clickbait) TP++; else FP++;
    }

    set.points[ROC_index].TPR = (double) TP / P;
    set.points[ROC_index].FPR = (double) FP / N;
    set.points[ROC_index].threshold = prev_prob;

    return set;
}

double calculate_AUC(ROC_set roc) {
    int i;
    double auc = 0.0, x_1, y_1, x_2, y_2;

    x_1 = roc.points[0].FPR;
    y_1 = roc.points[0].TPR;

    for (i = 1; i < roc.count; i++) {
        x_2 = roc.points[i].FPR;
        y_2 = roc.points[i].TPR;

        auc += 0.5 * (x_2 - x_1) * (y_2 + y_1);

        x_1 = x_2; y_1 = y_2;
    }

    return auc;
}

void write_ROC_file(ROC_set roc) {
    int i;
    FILE *fp;

    fp = fopen("roc_curve.csv", "w");

    fprintf(fp, "Threshold;FPR;TPR\n");

    for (i = 0; i < roc.count; i++) {
        fprintf(fp, "%s;%s;%s\n",
            double_to_string(roc.points[i].threshold, 6, ','),
            double_to_string(roc.points[i].FPR, 6, ','),
            double_to_string(roc.points[i].TPR, 6, ',')
        );
    }

    fclose(fp);
}

/* DEPRECATED
double calculate_ROC_AUC(Headline *headlines, int headline_count ) {
    double threshold, min_thres = 1, max_thres = 0, delta_thres;
    double last_fpr, last_tpr, ROC_auc = 0;
    FILE *fptr;

    fptr = fopen("roc_curve_old.csv", "w");

    fprintf(fptr, "Threshold;FPR;TPR\n");

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

        if (last_tpr != tpr || last_fpr != fpr) {
            fprintf(fptr, "%s;%s;%s\n",
                double_to_string(threshold, 6, ','),
                double_to_string(fpr, 6, ','),
                double_to_string(tpr, 6, ',')
            );
            
            ROC_auc += last_tpr * ( fpr - last_fpr ) + 0.5 * ( tpr - last_tpr ) * ( fpr - last_fpr );
            
            last_fpr = fpr;
            last_tpr = tpr;
        }
    }

    fclose(fptr);

    return ROC_auc;
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
*/

int _compare_probabilities(const void *pa, const void *pb) {
    Headline a = *(Headline*)pa, b = *(Headline*)pb;

    if (a.prob_cb < b.prob_cb) return 1;
    else if (a.prob_cb > b.prob_cb) return -1;
    else return 0;
}