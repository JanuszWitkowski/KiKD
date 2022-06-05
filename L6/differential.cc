#include "differential.hh"

double* filterAverage (uchar* x, size_t n) {
    double* filter = new double[n];
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = average(x[i], x[i-1]);
    }
    return filter;
}

double* filterDeviation (uchar* x, size_t n) {
    double* filter = new double[n];
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = deviation(x[i], x[i-1]);
    }
    return filter;
}
