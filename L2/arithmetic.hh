#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include "entropy.hh"
using namespace std;
typedef unsigned char uchar;

class ArithmeticCode {
private:
    int n;
    double marker;
public:
    ArithmeticCode(int length, double value);

    int n() {
        return n;
    }

    void n(int length) {
        n = length;
    }

    double marker() {
        return marker;
    }

    void marker(double value) {
        marker = value;
    }
};

ArithmeticCode *encode (uchar* array, int n);

uchar* decode (int n, double marker);

#endif