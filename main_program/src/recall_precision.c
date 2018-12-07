double recall(Headline *data_in, int data_amount){

    int i = 0;
    double clickbait_collected = 0;
    double clicbait_labled = 0;

     for (i; i < data_amount; i++){
         if(data_in[i].classified_clickbait == 1  && data_in[i].labeled_clickbait == 1){
                clickbait_collected++;
         }
         if (data_in[i].labeled_clickbait == 1) {
                clickbait_labeled++;
          }
     }
    return clickbait_collected / clickbait_labeled;
}

int precision(Headline *data_in, data_amount){
    int i = 0;
    double clickbait_collected = 0;
    double clickbait_clasified

    for (i; i < data_amount; i++){
         if(data_in[i].classified_clickbait == 1  && data_in[i].labeled_clickbait == 1){
                clickbait_collected++;
        }
        if (data_in[i].labeled_clickbait == 1) {
                clickbait_clasified++;
        }
    }
    return clickbait_collected / data_amount;

} 