#include "quantizer.hh"
#include <cmath>
#include <unordered_map>

Quantizer::Quantizer() {
    //
}

Quantizer::Quantizer(uchar* file, size_t n, int colorsNumber) {
    tga = new SimpleTGA(file, n);
    codebook = generateCodebook();
    bitmap = new PixelBitmap(file, tga->imageWidth, tga->imageHeight);
}

Quantizer::~Quantizer() {
    delete tga;
    delete codebook;
    delete bitmap;
}

uchar* Quantizer::encode() {
    for (size_t i = 0; i < bitmap->getHeight(); i++) {
        for (size_t j = 0; j < bitmap->getWidth(); j++) {
            double diffs[codebook->getSize()];
            PixelBitmap* pixelBitmap = new PixelBitmap(tga->getBitmap(), bitmap->getWidth(), bitmap->getHeight());
            for (size_t k = 0; k < codebook->getSize(); k++) {
                diffs[k] = euclidSquared(getPixelAsDoubleArray(tga->getPixelBitmap()->pixel(i, j)), getPixelAsDoubleArray(codebook->pixel(k)));
            }
            bitmap->pixel(i, j)->set(codebook->pixel(minIndexFromDoubles(diffs, codebook->getSize())));
        }
    }
    return tga->arrayToTGA(pixelbitmapToArray(bitmap), bitmap->getWidth() * bitmap->getHeight() * 3);
}

double Quantizer::mse() {
    double sum = 0.0;
    for (size_t i = 0; i < tga->imageHeight; i++) {
        for (size_t j = 0; j < tga->imageWidth; j++) {
            sum += euclidSquared(getPixelAsDoubleArray(tga->getPixelBitmap()->pixel(i, j)), getPixelAsDoubleArray(bitmap->pixel(i, j)));
        }
    }
    return sum / (tga->imageWidth * tga->imageHeight);
}

double Quantizer::snr(double error) {
    double sum = 0.0;
    for (size_t i = 0; i < tga->imageHeight; i++) {
        for (size_t j = 0; j < tga->imageWidth; j++) {
            sum += pow(tga->getPixelBitmap()->pixel(i, j)->B(), 2.0) +
                pow(tga->getPixelBitmap()->pixel(i, j)->G(), 2.0) +
                pow(tga->getPixelBitmap()->pixel(i, j)->R(), 2.0);
        }
    }
    return sum / (error * (double)(tga->imageWidth) * (double)(tga->imageHeight));
}

PixelArray* Quantizer::generateCodebook() {
    double epsilon = 0.00001;
    vector<double*> cb;
    vector<double*> data = castBitmapToVectors(tga->getPixelBitmap());
    double* c0 = avgVectorOfVectors(data);
    cb.push_back(c0);

    double avgDist  = avgDistortion(c0, data, data.size());

    while (cb.size() < codebook->getSize()) {
        double x;
        cb = splitCodebook(data, cb, epsilon, avgDist, x);
        avgDist = x;
    }
    return castCodebook(cb);
}

vector<double*> Quantizer::castBitmapToVectors(PixelBitmap* bitmap) {
    vector<double*> vectors;
    for (size_t i = 0; i < tga->imageHeight; i++) {
        for (size_t j = 0; j < tga->imageWidth; j++) {
            vectors.push_back(new double[3]{(double)(bitmap->pixel(i, j)->B()), 
                (double)(bitmap->pixel(i, j)->G()), 
                (double)(bitmap->pixel(i, j)->R())});
        }
    }
    return vectors;
}

PixelArray* Quantizer::castCodebook(vector<double*> vectors) {
    PixelArray* cb = new PixelArray(vectors.size());
    for (size_t i = 0; i < vectors.size(); i++) {
        cb->pixel(i)->set((uchar)(floor(vectors.at(i)[BLUE])),
                            (uchar)(floor(vectors.at(i)[GREEN])),
                            (uchar)(floor(vectors.at(i)[RED])));
    }
    return cb;
}

size_t Quantizer::minIndexFromDoubles(double array[], size_t n) {
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

double* Quantizer::avgVectorOfVectors(vector<double*> vectors) {
    size_t size = vectors.size();
    double* avgVec = new double[3]{0.0, 0.0, 0.0};
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < size; j++) {
            avgVec[i] += vectors.at(j)[i];
        }
        avgVec[i] /= size;
    }
    return avgVec;
}

double Quantizer::avgDistortion(double* vector0, vector<double*> vectors, size_t size) {
    vector<double> vectorsEuclid;
    vectorsEuclid.push_back(0.0);
    for (size_t i = 0; i < vectors.size(); i++) {
        vectorsEuclid.push_back(euclidSquared(vector0, vectors.at(i)));
    }
    // return vectorsEuclid ???
}

double Quantizer::avgDistortion(vector<double*> vectorsA, vector<double*> vectorsB, size_t size) {
    vector<double> vectorsEuclid;
    vectorsEuclid.push_back(0.0);
    for (size_t i = 0; i < vectorsA.size(); i++) {
        vectorsEuclid.push_back(euclidSquared(vectorsA.at(i), vectorsB.at(i)));
    }
    // return vectorsEuclid ???
}

double Quantizer::euclidSquared(double a[], double b[]) {
    double sum = 0.0;
    for (size_t i = 0; i < 3; i++) {
        sum += pow((double)(a[i] - b[i]), 2.0);
    }
    return sum;
}

vector<double*> Quantizer::splitCodebook(vector<double*> data, vector<double*> cb, double epsilon, double initialAvgDist, double &x) {
    size_t dataSize = data.size();
    vector<double*> newCB;
    for (size_t i = 0; i < cb.size(); i++) {
        newCB.push_back(newVector(cb.at(i), epsilon));
        newCB.push_back(newVector(cb.at(i), -epsilon));
    }
    cb = newCB;

    double avgDistortion = 0.0;
    double error = epsilon + 1;
    while (error > epsilon) {
        vector<double*> closest;
        for (size_t i = 0; i < dataSize; i++) {
            closest.push_back(nullptr);
        }
        // unordered_map<
    }
}

double* Quantizer::newVector(double vector[], double epsilon) {
    double newVector[3];
    for (size_t i = 0; i < 3; i++) {
        newVector[i] = vector[i] * (1.0 + epsilon);
    }
    return newVector;
}

double* Quantizer::getPixelAsDoubleArray(Pixel* a) {
    double x[3];
    x[0] = (double)(a->B());
    x[1] = (double)(a->G());
    x[2] = (double)(a->R());
    return x;
}
