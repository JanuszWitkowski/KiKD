#include "differential.hh"

double averageValue (uchar xn, uchar xm) { return ((double)xn + (double)xm)/2.0; }
double deviationValue (uchar xn, uchar xm) { return ((double)xn - (double)xm)/2.0; }


double* filterAverage (uchar* x, size_t n) {
    double* filter = new double[n];
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = averageValue(x[i], x[i-1]);
    }
    return filter;
}

double* filterDeviation (uchar* x, size_t n) {
    double* filter = new double[n];
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = deviationValue(x[i], x[i-1]);
    }
    return filter;
}


uchar quantize (double x, uchar* q, size_t qSize) {
    size_t left = 1, right = qSize - 1;
    while (left <= right) {
        size_t center = (left + right)/2;
        if (x < (double)(q[center])) {
            if (x >= (double)(q[center-1])) {
                return center - 1;
            }
            else {
                right = center - 1;
            }
        }
        else if (x > (double)(q[center])) {
            if (x <= (double)(q[center+1])) {
                return center;
            }
            else {
                left = center + 1;
            }
        }
        else {
            if (center == qSize-1) {
                return center - 1;
            }
            else {
                return center;
            }
        }
    }
    return -1;
}

uchar* differentialCoding (double* a, size_t aSize, size_t qBits) {
    // cout << "KODOWANIE ROZNICOWE\n";
    size_t qSize = (1 << qBits) + 1;
    uchar q[qSize];
    uchar qNext = (1 << (8 - qBits));
    uchar qHalf = (qNext >> 1);
    q[0] = 0;
    for (size_t i = 1; i <= qSize; i++)
        q[i] = q[i-1] + qNext;
    
    // cout << "ROZPOCZYNAM DIFFY\n";
    double diffs[aSize];
    uchar* d = new uchar[aSize];
    uchar aNew[aSize];
    diffs[0] = a[0];
    d[0] = quantize(diffs[0], q, qSize);
    aNew[0] = d[0];

    // cout << "ROZPOCZYNAM PETLE\n";
    for (size_t i = 1; i < aSize; i++) {
        // cout << i << "/" << aSize << endl;
        diffs[i] = a[i] - (double)(aNew[i-1]);
        // cout << "\tdiff = " << diffs[i] << endl;
        d[i] = quantize(diffs[i], q, qSize);
        // cout << "\td = " << (int)d[i] << endl;
        aNew[i] = q[d[i]] + qHalf - aNew[i-1];
        // cout << "\taNew = " << (int)aNew[i] << endl;;
    }

    // cout << "ZAKONCZONO ROZNICOWE\n";
    return d;
}

uchar* straightQuantizing (double* a, size_t aSize, size_t qBits) {
    size_t qSize = (1 << qBits) + 1;
    uchar q[qSize];
    uchar qNext = (1 << (8 - qBits));
    double delta = 255.0/2.0;
    q[0] = 0;
    for (size_t i = 1; i <= qSize; i++)
        q[i] = q[i-1] + qNext;
    uchar* output = new uchar[aSize];
    for (size_t i = 0; i < aSize; i++) {
        output[i] = quantize(a[i] + delta, q, qSize);
    }
    return output;
}
