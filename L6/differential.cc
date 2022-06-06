#include "differential.hh"

double averageValue (uchar xn, uchar xm) {
    // cout << (int)xn << "->" << (double)xn << " " << (int)xm << "->" << (double)xm << " =+= " << ((double)xn + (double)xm)/2.0 << endl;
    return ((double)xn + (double)xm)/2.0; 
}

double deviationValue (uchar xn, uchar xm) {
    // cout << (int)xn << "->" << (double)xn << " " << (int)xm << "->" << (double)xm << " =-= " << ((double)xn - (double)xm)/2.0 << endl;
    return ((double)xn - (double)xm)/2.0;
}


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


uchar quantize (double x, int* q, size_t qSize) {
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
    cerr << "ERROR: COS SIE SCHRZANILO Z KWANTYZACJA -- " << x << endl;
    return 0;
}

uchar* differentialCoding (double* a, size_t aSize, size_t qBits) {
    size_t qSize = (1 << qBits) + 1;
    int qinit[qSize];
    uchar qinitNext = (1 << (8 - qBits));
    qinit[0] = 0;
    for (size_t i = 1; i < qSize-1; i++) {
        qinit[i] = qinit[i-1] + qinitNext;
    }
    qinit[qSize-1] = -1;

    int q[qSize];
    uchar qNext = (1 << (9 - qBits));
    uchar qHalf = (qNext >> 1);
    q[0] = -256;
    for (size_t i = 1; i < qSize; i++) {
        q[i] = q[i-1] + qNext;
    }
    
    double diffs[aSize];
    uchar* d = new uchar[aSize];
    uchar aNew[aSize];
    diffs[0] = a[0];
    d[0] = quantize(diffs[0], qinit, qSize);
    aNew[0] = d[0];

    for (size_t i = 1; i < aSize; i++) {
        diffs[i] = a[i] - (double)(aNew[i-1]);
        d[i] = quantize(diffs[i], q, qSize);
        aNew[i] = q[d[i]] + qHalf - aNew[i-1];
    }

    return d;
}

uchar* straightQuantizing (double* a, size_t aSize, size_t qBits) {
    size_t qSize = (1 << qBits) + 1;
    int q[qSize];
    uchar qNext = (1 << (8 - qBits));
    double delta = 255.0/2.0;
    q[0] = 0;
    for (size_t i = 1; i < qSize-1; i++) {
        q[i] = q[i-1] + qNext;
    }
    q[qSize-1] = -1;
    uchar* output = new uchar[aSize];
    output[0] = quantize(a[0], q, qSize);
    for (size_t i = 1; i < aSize; i++) {
        output[i] = quantize(a[i] + delta, q, qSize);
    }
    return output;
}
