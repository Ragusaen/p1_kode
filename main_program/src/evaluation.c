#include "evaluation.h"

double recall(Headline *data_in, int data_amount) {

    int i;
    int clickbait_tp = 0;
    int clickbait_lp = 0;

/*Goes Through all the data.  */
    for (i = 0; i < data_amount; i++){
/*assign all the clickbait to clickbait_lp. */
        if ( data_in[i].labeled_clickbait ) {
            clickbait_lp++;

/*assign all the true positives in clickbait_lp to to clickbait_tp. */
            if ( data_in[i].classified_clickbait )
                clickbait_tp++;
        }
    }
    return (double)clickbait_tp / clickbait_lp;
}

double precision(Headline *data_in, int data_amount) {
    int i;
    int clickbait_cp = 0, clickbait_tp = 0;

/*Goes Through all the data.  */
    for (i = 0; i < data_amount; i++) {
/*assign all the positives to clickbait_cp. */
         if ( data_in[i].classified_clickbait ) {
            clickbait_cp++;

/*assign all the true positives in clickbait_cp to to clickbait_tp. */
            if ( data_in[i].labeled_clickbait )
                clickbait_tp++;
        }
    }
    return (double)clickbait_tp / clickbait_cp;

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
