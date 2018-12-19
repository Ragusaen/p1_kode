# Commands

#### train [path]
Train features on training set (path optional, if in config)

| Flag | Action |
| --- | --- |
| --print | print trained features |

#### test [path]
Run evaluation on test set (path optional, if in config)

| Flag | Action |
| --- | --- |
| --print | print evaluation data |
| --save [path] | export CSV data |

#### threshold [number]
Get saved threshold, number only used if flags set

| Flag | Action |
| --- | --- |
| --calc | calculate optimal threshold
| --from [dataset] | either 'test' (default) or 'training' |
| --print | print confusion matrix based on threshold |
| --save | save threshold to config |
 
#### classify "text"
Classify a single headline (text)

| Flag | Action |
| --- | --- |
| --threshold [n] | set new threshold (n) |



# Dataset information

TOTAL 474, TRAINING 332 (70.04%), TEST 142 (29.96%)


# Confusion matrix vocabulary

| Abbr | Description | Abbr | Description | Abbr | Description | Abbr | Description |
| ---: | ----------- | ---: | ----------- | ---: | ----------- | ---: | ----------- |
| CP | Condition positive | TP | True positive | PPV | Positive predictive value, *Precision | TPR | True positive rate, **Recall |
| CN | Condition negative | FP | False positive | FDR | False discovery rate | FPR | False positive rate, Fall-out |
| PCP | Predicted condition positive | FN | False negative | FOR | False omission rate | FNR | False negative rate, Miss rate |
| PCN | Predicted condition negative | TN | True negative | NPV | Negative predictive value | TNR | True negative rate, Selectivity |

| Abbr | Description |
| ---: | ----------- |
| ACC | Accuracy |
| LR+ | Positive likelihood ratio |
| LR- | Negative likelihood ratio |
| DOR | Diagnostic odds ratio |
| MCC | Matthews correlation coefficient [-1, 1] |