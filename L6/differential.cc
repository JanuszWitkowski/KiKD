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


uchar quantize (double x, uchar* q, size_t qSize, uchar qHalf) {
    size_t left = 1, right = qSize - 1;
    while (left <= right) {
        size_t center = (left + right)/2;
        if (x < (double)(q[center])) {
            if (x >= (double)(q[center-1])) {
                return q[center] - qHalf;
            }
            else {
                right = center;
            }
        }
        else if (x > (double)(q[center])) {
            if (x <= (double)(q[center+1])) {
                return q[center] + qHalf;
            }
            else {
                left = center;
            }
        }
        else {
            if (center == qSize-1) {
                return q[center] - qHalf;
            }
            else {
                return q[center] + qHalf;
            }
        }
    }
    return -1;
}

uchar* differentialCoding (double* a, size_t aSize, size_t qBits) {
    size_t qSize = (1 << qBits) + 1;
    uchar q[qSize];
    uchar qNext = (1 << (8 - qBits));
    uchar qHalf = (qNext >> 1);
    q[0] = 0;
    for (size_t i = 1; i <= qSize; i++)
        q[i] = q[i-1] + qNext;
    
    double diffs[aSize];
    uchar* d = new uchar[aSize];
    uchar aNew[aSize];
    diffs[0] = a[0];
    d[0] = quantize(diffs[0], q, qSize, qHalf);
    aNew[0] = d[0];

    for (size_t i = 1; i < aSize; i++) {
        diffs[i] = a[i] - (double)(aNew[i-1]);
        d[i] = quantize(diffs[i], q, qSize, qHalf);
        aNew[i] = d[i] - aNew[i-1];
    }
    return d;
}
