#ifndef DOUBLE_ARRAY_H
#define DOUBLE_ARRAY_H

#define _USE_MATH_DEFINES 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct DoubleArrayCalc {
    double sum, avg, min, max, median, lower, upper;
} DoubleArrayCalc;

DoubleArrayCalc double_array_calc(double arr[], int n);
void double_array_sort (double arr[], int n);
double double_array_sum(double arr[], int n);
double double_array_avg(double arr[], int n);
double double_array_min(double arr[]);
double double_array_max(double arr[], int n);
double double_array_median(double arr[], int n);
double double_array_lower(double arr[], int n);
double double_array_upper(double arr[], int n);

int _compare_doubles(const void *a, const void *b);

#endif