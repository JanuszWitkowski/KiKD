#ifndef __QUANTIZER_H
#define __QUANTIZER_H

#include <iostream>
#include <vector>
#include "tga.hh"
#include "pixel.hh"
#include "def.hh"

class Quantizer {
private:
    SimpleTGA* tga;
    PixelArray* codebook;
    PixelBitmap* bitmap;
    PixelArray* generateCodebook();
    vector<double*> castBitmapToVectors(PixelBitmap* bitmap);
    PixelArray* castCodebook(vector<double*> vectors);
    size_t minIndexFromDoubles(double array[], size_t n);
    double** bitmapToVectors(Pixel*** bitmap);
    Pixel** vectorsToBitmap(double** vectors);
    double* avgVectorOfVectors(vector<double*> vectors);
    double avgDistortion(double* vector0, vector<double*> vectors, size_t size);
    double avgDistortion(vector<double*> vectorsA, vector<double*> vectorsB, size_t size);
    double euclidSquared(double a[], double b[]);
    vector<double*> splitCodebook(vector<double*> data, vector<double*> cb, double epsilon, double initialAvgDist, double &x);
    double* newVector(double* vector, double epsilon);
    double* getPixelAsDoubleArray(Pixel* a);
public:
    Quantizer();
    Quantizer(uchar* file, size_t n, int colorsNumber);
    virtual ~Quantizer();
    uchar* encode();
    double mse();
    double snr(double error);
    SimpleTGA* getTGA() { return tga; }
    PixelArray* getCodebook() { return codebook; }
    PixelBitmap* getBitmap() { return bitmap; }
};

#endif