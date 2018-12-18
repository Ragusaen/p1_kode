#include "core/test.h"

/* internal functions */
static ResultCounter _count_thresholds_positives_negatives(DataSet);
static ResultCounter _count_true_false_positives(DataSet);
static ConfusionMatrix _calc_confusion_matrix(int, int, int, int, double);
static int _sort_by_probability_desc(const void *, const void *);
static void _write_evaluation_data(FILE *, ConfusionMatrix);
static char* _csv_double(double);


/**
 * Evaluates the classifier at all thresholds, outputs an EvaluationSet with an array of Confusion Matrixes.
 * 
 * @param dataset   a dataset with calculated probabilities
 */

EvaluationSet test_classifier(DataSet dataset)
{
    int i, output_i = 0;
    double threshold = 1;
    EvaluationSet evaluation;
    ResultCounter c;

    /* sort the dataset from highest to lowest probability */
    qsort(dataset.data, dataset.count, sizeof(Headline), _sort_by_probability_desc);

    /* count the number of thresholds (unique probabilities), positives, and negatives */
    c = _count_thresholds_positives_negatives(dataset);
    
    /* initialize the EvaluationSet */
    evaluation.count = c.thresholds;
    if ((evaluation.data = (ConfusionMatrix*) calloc(evaluation.count, sizeof(ConfusionMatrix))) == NULL) fatal_error();
    
    for (i = 0; i < dataset.count; i++) {
        /* if the probability is not equal to current threshold */
        if (dataset.data[i].prob_score != threshold) {
            /* add ConfusionMatrix with current counts to EvaluationSet */
            evaluation.data[output_i++] = _calc_confusion_matrix(c.P, c.N, c.TP, c.FP, threshold);
            /* set new threshold */
            threshold = dataset.data[i].prob_score;
        }

        /* count item as either TP or FP */
        if (dataset.data[i].labeled_clickbait) c.TP++; else c.FP++;
    }

    /* add the last ConfusionMatrix with the final count */
    evaluation.data[output_i] = _calc_confusion_matrix(c.P, c.N, c.TP, c.FP, threshold);

    return evaluation;
}


/**
 * Evaluates a specific classification.
 * 
 * @param dataset       a classified dataset
 * @param threshold     will be passed to the ConfusionMatrix
 */

ConfusionMatrix test_classification(DataSet dataset, double threshold)
{
    ResultCounter result;
    ConfusionMatrix cm;

    /* count all positives, negatives, and all true and false positives */
    result = _count_true_false_positives(dataset);
    
    /* create Confusion Matrix */
    cm = _calc_confusion_matrix(result.P, result.N, result.TP, result.FP, threshold);

    return cm;
}


/**
 * Calculates the area under the ROC curve.
 * 
 * @param evaluation   an EvaluationSet which contains an array of Confusion Matrixes.
 */

double test_calculate_auc(EvaluationSet evaluation)
{
    int i;
    double auc = 0.0, x_1, y_1, x_2, y_2;

    /* set first point */
    x_1 = evaluation.data[0].FPR;
    y_1 = evaluation.data[0].TPR;

    for (i = 1; i < evaluation.count; i++) {
        /* set second point */
        x_2 = evaluation.data[i].FPR;
        y_2 = evaluation.data[i].TPR;

        /* add area under points to sum */
        auc += 0.5 * (x_2 - x_1) * (y_2 + y_1);

        /* set first point to second points coordinates */
        x_1 = x_2;
        y_1 = y_2;
    }

    return auc;
}


/**
 * Exports the EvaluationSet to a CSV file.
 * 
 * @param evaluation    an EvaluationSet created by test_classifier
 * @param filename      the path and filename for the CSV file
 */

void test_export_file(EvaluationSet evaluation, const char *filename)
{
    int i;
    FILE *fp;

    if((fp = fopen(filename, "w")) == NULL) fatal("Couldn't write to evaluation file");

    /* write header row */
    fprintf(fp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
        "Threshold", "TP", "FP", "FN", "TN", "Accuracy", "Precision", "Recall", "Fall-out", "F1 score", "MCC"
    );

    /* write data */
    for (i = 0; i < evaluation.count; i++) {
        _write_evaluation_data(fp, evaluation.data[i]);
    }

    fclose(fp);
}


/**
 * Counts the number of thresholds (unique probabilities), positives, and negatives
 */

ResultCounter _count_thresholds_positives_negatives(DataSet dataset)
{
    int i;
    double threshold = 1;
    ResultCounter c;

    /* reset counts */
    c.P = 0; c.N = 0; c.TP = 0; c.FP = 0; c.thresholds = 1;

    for (i = 0; i < dataset.count; i++) {
        /* count unique probabilities (thresholds) */
        if (dataset.data[i].prob_score != threshold) {
            threshold = dataset.data[i].prob_score;
            c.thresholds++;
        }
        
        /* count item as either positive or negative */
        if (dataset.data[i].labeled_clickbait) c.P++; else c.N++;
    }

    return c;
}


/**
 * Counts all positives, negatives, and all true and false positives.
 * Requires a classified dataset.
 */

ResultCounter _count_true_false_positives(DataSet dataset)
{
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


/**
 * Calculates and returns a Confusion Matrix.
 * 
 * @param P     number of positives
 * @param N     number of negatives
 * @param TP    number of true positives
 * @param FP    number of false positives
 * @threshold   the threshold used
 */

ConfusionMatrix _calc_confusion_matrix(int P, int N, int TP, int FP, double threshold)
{
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


/**
 * A comparison function for use with qsort.
 * Sorts headlines by probability in descending order.
 */

int _sort_by_probability_desc(const void *pa, const void *pb)
{
    Headline a = *(Headline*)pa, b = *(Headline*)pb;

    if (a.prob_score < b.prob_score) return 1;
    else if (a.prob_score > b.prob_score) return -1;
    else return 0;
}


/**
 * Writes a ConfusionMatrix to a CSV file.
 */

void _write_evaluation_data(FILE *fp, ConfusionMatrix data)
{
    fprintf(fp, "%s;%d;%d;%d;%d;%s;%s;%s;%s;%s;%s\n",
        _csv_double(data.threshold),
        data.TP, data.FP, data.FN, data.TN,
        _csv_double(data.ACC),
        _csv_double(data.PPV),
        _csv_double(data.TPR),
        _csv_double(data.FPR),
        _csv_double(data.F1),
        _csv_double(data.MCC)
    );
}


/**
 * Converts a double to a string with ',' as the decimal separator.
 */

char* _csv_double(double n)
{
    char *str, *token;
    
    if ((str = malloc(16)) == NULL) fatal_error();

    /* convert double to string */
    sprintf(str, "%.6f", n);

    /* replace the decimal separator */
    token = strchr(str, '.');
    token[0] = ',';

    return str;
}