#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <vector>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "entropy.hh"
using namespace std;
typedef unsigned char uchar;
// using boost::multiprecision::cpp_dec_float_50;
using boost::multiprecision::cpp_dec_float_100;
// using boost::multiprecision::cpp_dec_float;

// typedef cpp_dec_float_50 dec;
// typedef cpp_dec_float_100 fdec;
// typedef boost::multiprecision::number<cpp_dec_float<200> > fdec;
typedef double fdec;

class ArithmeticCode {
private:
    int n;
    // double tag;
    fdec tag;
    // vector<fdec> tags;

public:
ArithmeticCode(int length, fdec tag);
    // ArithmeticCode(int length, vector<fdec> vector);
    // virtual ~ArithmeticCode (void) {
    //     vector<fdec>().swap(tags);
    // }

    int getN() {
        return n;
    }

    void setN(int length) {
        n = length;
    }

    fdec getTag() {
        return tag;
    }

    void setTag(fdec value) {
        tag = value;
    }

    // void insertTag (fdec tag) {
    //     tags.push_back(tag);
    // }

    // fdec getTag (int index) {
    //     return tags[index];
    // }

    // int getTagsSize () {
    //     return tags.size();
    // }
};

uchar* setBlock (uchar* array, uchar* block, int start, int m, int n);
void updateCharOccs (int* occs, uchar* array, int n);
int sumOfOccs (int* occs, uchar symbol);
unsigned int countBuffer (int* occs, int n);

ArithmeticCode *encode (uchar* array, int n, int b, string codename);
uchar* decode (int n, string tag);

void compress (string filename, string codename);
void decompress (string codename, string filename);
bool compareFiles (string file1, string file2);

#endif