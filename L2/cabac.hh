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
    double tag;
public:
    ArithmeticCode(int length, double value);

    int n() {
        return n;
    }

    void n(int length) {
        n = length;
    }

    double tag() {
        return tag;
    }

    void tag(double value) {
        tag = value;
    }
};

uchar* setBlock (uchar* array, uchar* block, int start, int m, int n);
void updateCharOccs (int* occs, uchar* array, int n);
double sumOfOccs (int* occs, uchar symbol);

ArithmeticCode *encode (uchar* array, int n);
uchar* decode (int n, double tag);

void compress (string filename, string codename);
void decompress (string codename, string filename);
void compareFiles (string file1, string file2);

#endif