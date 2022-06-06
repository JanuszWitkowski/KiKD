#ifndef __DIFFERENTIAL_H
#define __DIFFERENTIAL_H

#include <iostream>
#include "consts.hh"

using namespace std;

double averageValue (uchar xn, uchar xm);
double deviationValue (uchar xn, uchar xm);

double* filterAverage (uchar* x, size_t n);
double* filterDeviation (uchar* x, size_t n);

uchar* differentialCoding (double* a, size_t aSize, size_t qBits);
uchar* straightQuantizing (double* a, size_t aSize, size_t qBits);

int differentialDecoding (uchar* a, size_t aSize, size_t qBits);
int straightDequantizing (uchar* a, size_t aSize, size_t qBits);

#endif