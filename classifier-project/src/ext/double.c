#include "ext/double.h"

/* internal functions */
static int _compare_doubles(const void *, const void *);


/**
 * Calculate all statistics from double array
 */

DoubleArrayCalc double_array_calc(double *arr, int n)
{
    DoubleArrayCalc calc;

    double_array_sort(arr, n);
    
    calc.sum = double_array_sum(arr, n);
    calc.avg = calc.sum / n;
    calc.min = arr[0];
    calc.max = arr[n - 1];
    calc.median = double_array_median(arr, n);
    calc.lower = double_array_lower(arr, n);
    calc.upper = double_array_upper(arr, n);

    return calc;
}


/**
 * Sort array ascending
 */

void double_array_sort(double *arr, int n)
{
    qsort(arr, n, sizeof(double), _compare_doubles);
}


/**
 * Calculate sum
 */

double double_array_sum(double *arr, int n)
{
    int i;
    double sum = 0;

    for (i = 0; i < n; i++)
        sum += arr[i];

    return sum;
}


/**
 * Calculate average
 */

double double_array_avg(double *arr, int n)
{
    return double_array_sum(arr, n) / n;
}


/**
 * Find minimum
 */

double double_array_min(double *arr)
{
    return arr[0];
}


/**
 * Find maximum
 */

double double_array_max(double *arr, int n)
{
    return arr[n - 1];
}


/**
 * Calculate median
 */

double double_array_median(double *arr, int n)
{
    double median = 0;

    if (n % 2 == 0)
        median = (arr[(n - 1) / 2] + arr[n / 2]) / 2.0;
    else
        median = arr[n / 2];

    return median;
}


/**
 * Calculate lower quartile
 */

double double_array_lower(double *arr, int n)
{
    double lower = 0;

    if (n % 4 == 0)
        lower = arr[(int)(n * 0.25)];
    else
        lower = (arr[(int)floor((double)n * 0.25)] + arr[(int)ceil((double)n * 0.25)]) / 2.0;

    return lower;
}


/**
 * Calculate upper quartile
 */

double double_array_upper(double *arr, int n)
{
    double lower = 0;

    if (n % 4 == 0)
        lower = arr[(int)(n * 0.75)];
    else
        lower = (arr[(int)floor((double)n * 0.75)] + arr[(int)ceil((double)n * 0.75)]) / 2.0;

    return lower;
}


/**
 * Compare doubles, sort ascending
 */

int _compare_doubles(const void *a, const void *b)
{
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}