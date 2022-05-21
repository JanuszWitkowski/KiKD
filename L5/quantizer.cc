#include "quantizer.hh"

Quantizer::Quantizer() {
    //
}

Quantizer::Quantizer(uchar* file, size_t n, int colorsNumber) {
    tga = new SimpleTGA(file, n);
    codebookSize = colorsNumber;
    codebook = generateCodebook();
    bitmap = new PixelBitmap(file, tga->imageWidth, tga->imageHeight);
}

Quantizer::~Quantizer() {
    delete tga;
    for (size_t i = 0; i < codebookSize; i++)
        delete codebook[i];
    delete codebook;
    delete bitmap;
}

uchar* Quantizer::encode() {
    for (size_t i = 0; i < bitmap->getHeight(); i++) {
        for (size_t j = 0; j < bitmap->getWidth(); j++) {
            double diffs[codebookSize];
            PixelBitmap* pixelBitmap = new PixelBitmap(tga->getBitmap(), bitmap->getWidth(), bitmap->getHeight());
            for (size_t k = 0; k < codebookSize; k++) {
                diffs[k] = euclidSquared(getPixelAsDoubleArray(pixelBitmap->pixel(i, j)), getPixelAsDoubleArray(codebook[k]));
            }
            bitmap->pixel(i, j)->set(codebook[minIndexFromDoubles(diffs, codebookSize)]);
        }
    }
    return tga->arrayToTGA(pixelbitmapToArray(bitmap), bitmap->getWidth() * bitmap->getHeight());
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

size_t minIndexFromDoubles(double array[], size_t n) {
    size_t index = 0;
    uchar min = -1;
    for (size_t i = 0; i < n; i++) {
        if (array[i] < min) {
            min = array[i];
            index = i;
        }
    }
    return index;
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
