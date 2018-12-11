#include "recall_precision.h"
#include "headline.h"

double recall(Headline *data_in, int data_amount) {

    int i;
    int clickbait_tp = 0;
    int clickbait_lp = 0;

    for (i = 0; i < data_amount; i++){
        if ( data_in[i].labeled_clickbait ) {
            clickbait_lp++;

            if ( data_in[i].classified_clickbait )
                clickbait_tp++;
        }
    }
    return (double)clickbait_tp / clickbait_lp;
}

double precision(Headline *data_in, int data_amount) {
    int i;
    int clickbait_cp = 0, clickbait_tp = 0;

    for (i = 0; i < data_amount; i++) {
         if ( data_in[i].classified_clickbait ) {
            clickbait_cp++;

            if ( data_in[i].labeled_clickbait )
                clickbait_tp++;
        }
    }
    return (double)clickbait_tp / clickbait_cp;

}
