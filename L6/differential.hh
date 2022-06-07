#ifndef __DIFFERENTIAL_H
#define __DIFFERENTIAL_H

#include <iostream>
#include "consts.hh"
#include "bitrw.hh"

using namespace std;

double averageValue (uchar xn, uchar xm);
double deviationValue (uchar xn, uchar xm);

double* filterAverage (uchar* x, size_t n);
double* filterDeviation (uchar* x, size_t n);

uchar* differentialCoding (double* a, size_t aSize, size_t qBits);
uchar* straightQuantizing (double* a, size_t aSize, size_t qBits);

void printBandsToFile (string filename, uchar** downs, uchar** ups, size_t width, size_t height, size_t qBits);

int differentialDecoding (uchar* a, size_t aSize, size_t qBits);
int straightDequantizing (uchar* a, size_t aSize, size_t qBits);


class BandSolver {
private:
    int*** filters;
    uchar*** codings;
    uchar* bitmap;
    size_t bandsNumber = 2;
    size_t colorsNumber = 3;
    size_t length;
    size_t width, height, qBits;
public:
    BandSolver();
    BandSolver(string filename);
    virtual ~BandSolver();
    size_t getLength() { return length; }
    size_t getBandsNumber() { return bandsNumber; }
    size_t gettColorsNumber() { return colorsNumber; }
    int* colorFilter(size_t band, ColorBGRType color) { return filters[band][color]; }
    uchar* colorCoding(size_t band, ColorBGRType color) { return codings[band][color]; }
    uchar* getBitmap() { return bitmap; }
    uchar getBitmapAt(size_t i) { return bitmap[i]; }
};

#endif