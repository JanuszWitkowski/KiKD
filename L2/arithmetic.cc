#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

#include "entropy.hh"
#include "arithmetic.hh"
using namespace std;

typedef unsigned char uchar;

ArithmeticCode::ArithmeticCode(int length, double value) {
    n(length);
    marker(value);
}

ArithmeticCode *encode (uchar* array, int n) {
    double marker = 0.0;
    return new ArithmeticCode(n, marker);
}

uchar* decode (int n, double marker) {
    uchar* array = new uchar[n];
    return array;
}
