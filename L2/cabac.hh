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

    int getN() {
        return n;
    }

    void setN(int length) {
        n = length;
    }

    double getTag() {
        return tag;
    }

    void setTag(double value) {
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
bool compareFiles (string file1, string file2);

#endif