#ifndef __QUANTIZER_H
#define __QUANTIZER_H

#include <iostream>
#include "tga.hh"
#include "pixel.hh"
#include "def.hh"

class Quantizer {
private:
    size_t codebookSize;
    size_t bitmapWidth;
    size_t bitmapHeight;
    SimpleTGA* tga;
    Pixel** codebook;
    Pixel*** bitmap;
    Pixel** generateCodebook();
    double** bitmapToVectors(Pixel*** bitmap);
    Pixel** vectorsToBitmap(double** vectors);
    double* avgVectorOfVectors(double** vectors);
    double avgDistortion(double* vector0, double** vectors, size_t size);
    double avgDistortion(double** vectorsA, double** vectorsB, size_t size);
    double euclidSquared(double* a, double* b);
    double** splitCodebook(double** data, double** codebook, double epsilon, double initialAvgDist, double &x);
    double* newVector(double* vector, double epsilon);
    double* getPixelAsDoubleArray(Pixel* a);
public:
    Quantizer();
    Quantizer(uchar* file, size_t n, int colorsNumber);
    virtual ~Quantizer();
    uchar* encode();
    double mse();
    double snr();
    SimpleTGA* getTGA() { return tga; }
    Pixel** getCodebook() { return codebook; }
    Pixel*** getBitmap() { return bitmap; }
};

#endif