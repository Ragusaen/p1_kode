#include "recall_precision.h"
#include "headline.h"

double recall(Headline *data_in, int data_amount) {

    int i;
    int clickbait_collected = 0;
    int clickbait_labeled = 0;

    for (i = 0; i < data_amount; i++){
         if(data_in[i].classified_clickbait == 1  && data_in[i].labeled_clickbait == 1){
             clickbait_collected++;
         }
         if (data_in[i].labeled_clickbait == 1) {
             clickbait_labeled++;
         }
    }
    return clickbait_collected / clickbait_labeled;
}

double precision(Headline *data_in, int data_amount) {
    int i = 0;
    double clickbait_collected = 0;
    double clickbait_clasified = 0;

    for (i; i < data_amount; i++){
         if(data_in[i].classified_clickbait == 1  && data_in[i].labeled_clickbait == 1){
                clickbait_collected++;
        }
        if (data_in[i].labeled_clickbait == 1) {
                clickbait_clasified++;
        }
    }
    return clickbait_collected / clickbait_clasified;

}
