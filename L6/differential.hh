#ifndef __DIFFERENTIAL_H
#define __DIFFERENTIAL_H

#include <iostream>
#include "consts.hh"

using namespace std;

double average (uchar xn, uchar xm) { return ((double)xn + (double)xm)/2.0; }
double deviation (uchar xn, uchar xm) { return ((double)xn - (double)xm)/2.0; }

double* filterAverage (uchar* x, size_t n);
double* filterDeviation (uchar* x, size_t n);

#endif