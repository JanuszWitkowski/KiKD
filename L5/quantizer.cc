#include "quantizer.hh"

Quantizer::Quantizer() {
    //
}

Quantizer::Quantizer(uchar* file, size_t n, int colorsNumber) {
    //
}

Quantizer::~Quantizer() {
    delete tga;
    for (size_t i = 0; i < codebookSize; i++)
        delete codebook[i];
    delete codebook;
    for (size_t i = 0; i < bitmapHeight; i++) {
        for (size_t j = 0; j < bitmapWidth; j++) {
            delete bitmap[i][j];
        }
        delete[] bitmap[i];
    }
    delete[] bitmap;
}

uchar* Quantizer::encode() {
    uchar* output = new uchar[1];
    return output;
}

double Quantizer::mse() {
    double mse = 1.0;
    return mse;
}

double Quantizer::snr() {
    double snr = 1.0;
    return snr;
}

Pixel** Quantizer::generateCodebook() {
    Pixel** cb = new Pixel*[1];
    return cb;
}

double** Quantizer::bitmapToVectors(Pixel*** bitmap) {
    double** vectors = new double*[1];
        vectors[0] = new double[3];
    return vectors;
}

Pixel** Quantizer::vectorsToBitmap(double** vectors) {
    Pixel** bm = new Pixel*[1];
    return bm;
}

double* Quantizer::avgVectorOfVectors(double** vectors) {
    double* avgVec = new double[1];
    return avgVec;
}

double Quantizer::avgDistortion(double* vector0, double** vectors, size_t size) {
    double avgDist = 1.0;
    return avgDist;
}

double Quantizer::avgDistortion(double** vectorsA, double** vectorsB, size_t size) {
    double avgDist = 1.0;
    return avgDist;
}

double Quantizer::euclidSquared(double* a, double* b) {
    double euc = 1.0;
    return euc;
}

double** Quantizer::splitCodebook(double** data, double** codebook, double epsilon, double initialAvgDist, double &x) {
    double** split = new double*[1];
        split[0] = new double[1];
    return split;
}

double* Quantizer::newVector(double* vector, double epsilon) {
    double* newVec = new double[1];
    return newVec;
}

double* Quantizer::getPixelAsDoubleArray(Pixel* a) {
    double* array = new double[1];
    return array;
}
