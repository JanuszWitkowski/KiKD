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

ArithmeticCode *encode (File *file) {
    double marker = 0.0;
    return new ArithmeticCode(file->length(), marker);
}

uchar* decode (int n, double marker) {
    uchar* array = new uchar[n];
    return array;
}
