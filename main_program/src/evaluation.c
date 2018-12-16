#include "evaluation.h"

EvaluationSet evaluate_classifier(DataSet dataset) {
    int i, output_i;
    double prev_prob = 1;
    EvaluationSet set;
    ResultCounter c;

    qsort(dataset.data, dataset.count, sizeof(Headline), _sort_by_probability_desc);

    c = _count_thresholds_positives_negatives(dataset);
    
    set.count = c.thresholds;
    set.data = (ConfusionMatrix*) calloc(set.count, sizeof(ConfusionMatrix));
    
    for (i = 0; i < dataset.count; i++) {
        if (dataset.data[i].prob_cb != prev_prob) {
            set.data[output_i++] = _calc_confusion_matrix(c.P, c.N, c.TP, c.FP, prev_prob);
            prev_prob = dataset.data[i].prob_cb;
        }

        if (dataset.data[i].labeled_clickbait) c.TP++; else c.FP++;
    }

    set.data[output_i] = _calc_confusion_matrix(c.P, c.N, c.TP, c.FP, prev_prob);

    return set;
}

ConfusionMatrix evaluate_classification(DataSet dataset, double threshold) {
    ResultCounter result = _count_true_false_positives(dataset);
    
    ConfusionMatrix cm = _calc_confusion_matrix(result.P, result.N, result.TP, result.FP, threshold);

    return cm;
}


/**
 * Calculates the area under the ROC curve.
 * 
 * @param set   contains a data array of Confusion Matrixes.
 */

double calculate_AUC(EvaluationSet set) {
    int i;
    double auc = 0.0, x_1, y_1, x_2, y_2;

    /* set first point */
    x_1 = set.data[0].FPR;
    y_1 = set.data[0].TPR;

    for (i = 1; i < set.count; i++) {
        /* set second point */
        x_2 = set.data[i].FPR;
        y_2 = set.data[i].TPR;

        /* add area under points to sum */
        auc += 0.5 * (x_2 - x_1) * (y_2 + y_1);

        /* set first point to second points coordinates */
        x_1 = x_2;
        y_1 = y_2;
    }

    return auc;
}

void write_evaluation_file(EvaluationSet set, char *filename) {
    int i, col = 0;
    FILE *fp;
    CSV_data row[11]; /* data array with 11 columns */

    if((fp = fopen(filename, "w")) == NULL) {
        printf("Error: Couldn't open file \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }

    /* write header row */
    strcpy(row[col++].s, "Threshold");
    strcpy(row[col++].s, "TP");
    strcpy(row[col++].s, "FP");
    strcpy(row[col++].s, "FN");
    strcpy(row[col++].s, "TN");
    strcpy(row[col++].s, "Accuracy");
    strcpy(row[col++].s, "Precision");
    strcpy(row[col++].s, "Recall");
    strcpy(row[col++].s, "Fall-out");
    strcpy(row[col++].s, "F1 score");
    strcpy(row[col++].s, "MCC");
    write_csv_line(fp, "%s %s %s %s %s %s %s %s %s %s %s", row);

    /* write data */
    for (i = 0; i < set.count; i++) {
        col = 0;
        row[col++].f = set.data[i].threshold;
        row[col++].d = set.data[i].TP;
        row[col++].d = set.data[i].FP;
        row[col++].d = set.data[i].FN;
        row[col++].d = set.data[i].TN;
        row[col++].f = set.data[i].ACC;
        row[col++].f = set.data[i].PPV;
        row[col++].f = set.data[i].TPR;
        row[col++].f = set.data[i].FPR;
        row[col++].f = set.data[i].F1;
        row[col++].f = set.data[i].MCC;
        write_csv_line(fp, "%f %d %d %d %d %f %f %f %f %f %f", row);
    }

    fclose(fp);
}

ResultCounter _count_thresholds_positives_negatives(DataSet dataset) {
    int i;
    double prev_prob = 1;
    ResultCounter c;

    c.P = 0; c.N = 0; c.TP = 0; c.FP = 0; c.thresholds = 1;

    for (i = 0; i < dataset.count; i++) {
        if (dataset.data[i].labeled_clickbait) c.P++; else c.N++;
        if (dataset.data[i].prob_cb != prev_prob) {
            prev_prob = dataset.data[i].prob_cb;
            c.thresholds++;
        }
    }

    return c;
}

ResultCounter _count_true_false_positives(DataSet dataset) {
    int i;
    uint8_t cls, lbl;
    ResultCounter c;

    /* reset counters */
    c.P = 0; c.N = 0; c.TP = 0; c.FP = 0;

    for (i = 0; i < dataset.count; i++) {
        lbl = dataset.data[i].labeled_clickbait;
        cls = dataset.data[i].classified_clickbait;

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

ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP, double threshold) {
    double mcc_denom;
    ConfusionMatrix cm;

    cm.threshold = threshold;
    
    cm.P = P; cm.N = N; cm.TP = TP; cm.FP = FP;

    cm.FN = cm.P - cm.TP;   /* false negatives */
    cm.TN = cm.N - cm.FP;   /* true negatives */
    cm.PP = cm.TP + cm.FP;  /* predicted condition positive */
    cm.PN = cm.FN + cm.TN;  /* predicted condition negative */
    cm.total = cm.P + cm.N; /* total population */

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
    cm.PPV = cm.PP != 0 ? (double) cm.TP / cm.PP : 0;
    /* negative predictive value (NPV) */
    cm.NPV = cm.PN != 0 ? (double) cm.TN / cm.PN : 0;
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

int _sort_by_probability_desc(const void *pa, const void *pb) {
    Headline a = *(Headline*)pa, b = *(Headline*)pb;

    if (a.prob_cb < b.prob_cb) return 1;
    else if (a.prob_cb > b.prob_cb) return -1;
    else return 0;
}