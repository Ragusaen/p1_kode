#ifndef DOUBLE_H
#define DOUBLE_H

#define _USE_MATH_DEFINES 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct DoubleArrayCalc {
    double sum, avg, min, max, median, lower, upper;
} DoubleArrayCalc;


DoubleArrayCalc double_array_calc(double *, int);
void double_array_sort (double *, int);

double double_array_sum(double *, int);
double double_array_avg(double *, int);
double double_array_min(double *);
double double_array_max(double *, int);
double double_array_median(double *, int);
double double_array_lower(double *, int);
double double_array_upper(double *, int);

#endif