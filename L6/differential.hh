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


class FilterHolder {
private:
    double** filter;
    size_t colorsNumber = 3;
    size_t length;
public:
    FilterHolder(size_t size);
    virtual ~FilterHolder();
    double** getFilter() { return filter; }
    double getFilterAt(ColorBGRType color, size_t i) { return filter[color][i]; }
    void setFilterAt(ColorBGRType color, size_t i, double v) { filter[color][i] = v; }
    void setFilter(size_t color, double* array) { filter[color] = array; }
};

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
    BandSolver(size_t size);
    BandSolver(string filename);
    virtual ~BandSolver();
    size_t getLength() { return length; }
    size_t getBandsNumber() { return bandsNumber; }
    size_t getColorsNumber() { return colorsNumber; }
    size_t getWidth() { return width; }
    size_t getHeight() { return height; }
    size_t getBits() { return qBits; }
    uchar** getCoding(size_t band) { return codings[band]; }
    int* colorFilter(size_t band, ColorBGRType color) { return filters[band][color]; }
    uchar* colorCoding(size_t band, ColorBGRType color) { return codings[band][color]; }
    uchar* getBitmap() { return bitmap; }
    uchar getBitmapAt(size_t i) { return bitmap[i]; }
    void setCodingAt(size_t band, size_t color, size_t i, uchar v) { codings[band][color][i] = v; }
    void setCodingBand(size_t band, size_t color, uchar* array) { codings[band][color] = array; }
};

#endif