#include "quantizer.hh"
#include <cmath>
#include <unordered_map>
#include <algorithm>

Quantizer::Quantizer() {
    //
}

Quantizer::Quantizer(uchar* file, size_t n, int colorsNumber) {
    tga = new SimpleTGA(file, n);
    codebook = generateCodebook(colorsNumber);
    bitmap = new PixelBitmap(file, tga->imageWidth, tga->imageHeight);
}

Quantizer::~Quantizer() {
    delete tga;
    delete codebook;
    delete bitmap;
}

uchar* Quantizer::encode(string codename) {
    for (size_t i = 0; i < bitmap->getHeight(); i++) {
        for (size_t j = 0; j < bitmap->getWidth(); j++) {
            double diffs[codebook->getSize()];
            for (size_t k = 0; k < codebook->getSize(); k++) {
                double* pixelOld = getPixelAsDoubleArray(bitmap->pixel(i, j));
                double* pixelNew = getPixelAsDoubleArray(codebook->pixel(k));
                diffs[k] = euclidSquared(pixelOld, pixelNew);
                delete[] pixelOld;
                delete[] pixelNew;
            }
            bitmap->pixel(i, j)->set(codebook->pixel(minIndexFromDoubles(diffs, codebook->getSize())));
        }
    }

    uchar* bitmapArray = pixelbitmapToArray(bitmap);
    printArrayToFile("output/array.txt", bitmapArray, bitmap->getWidth(), bitmap->getHeight());
    size_t tgaSize = bitmap->getWidth() * bitmap->getHeight() * 3;
    uchar* tgaArray = tga->arrayToTGA(bitmapArray, tgaSize);
    tga->produceTGAFile(codename, tgaArray, tgaSize);
    delete[] bitmapArray;
    return tgaArray;
}

double Quantizer::mse() {
    double sum = 0.0;
    for (size_t i = 0; i < tga->imageHeight; i++) {
        for (size_t j = 0; j < tga->imageWidth; j++) {
            double* pixelOld = getPixelAsDoubleArray(tga->getPixelBitmap()->pixel(i, j));
            double* pixelNew = getPixelAsDoubleArray(bitmap->pixel(i, j));
            sum += euclidSquared(pixelOld, pixelNew);
            delete[] pixelOld;
            delete[] pixelNew;
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

PixelArray* Quantizer::generateCodebook(int codebookSize) {
    cout << "generateCodebook" << endl;
    double epsilon = 0.00001;
    vector<double*> cb;
    vector<double*> data = castBitmapToVectors(tga->getPixelBitmap());
    double* c0 = avgVectorOfVectors(data);
    cb.push_back(c0);

    double avgDist  = avgDistortion(c0, data, data.size());

    size_t counter = 0;
    cout << "Splitting" << endl;
    while (cb.size() < codebookSize) {
        counter++;
        double x;
        cout << "Split " << counter << endl;
        cb = splitCodebook(data, cb, epsilon, avgDist, x);
        avgDist = x;
    }
    cout << counter << "-" << cb.size() << endl;
    PixelArray* newCB = castCodebook(cb);

    /* CLEANUP */
    for (size_t i = 0; i < cb.size(); i++)
        delete[] cb.at(i);
    for (size_t i = 0; i < data.size(); i++)
        delete[] data.at(i);
    
    return newCB;
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
    double distortion = 0.0;
    for (size_t i = 0; i < vectors.size(); i++) {
        distortion += euclidSquared(vector0, vectors.at(i));
    }
    return distortion/size;
}

double Quantizer::avgDistortion(vector<double*> vectorsA, vector<double*> vectorsB, size_t size) {
    double distortion = 0.0;
    for (size_t i = 0; i < vectorsA.size(); i++) {
        distortion += euclidSquared(vectorsA.at(i), vectorsB.at(i));
    }
    return distortion/size;
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

    double averageDistortion = 0.0;
    double error = epsilon + 1;
    while (error > epsilon) {
        vector<double*> closest;
        for (size_t i = 0; i < dataSize; i++) {
            closest.push_back(nullptr);
        }
        // vector<vector<double*>> nearestVectors;
        // vector<vector<int>> nearestVectorsIndexes;
        unordered_map<int, vector<double*>> nearestVectors;
        unordered_map<int, vector<int>> nearestVectorsIndexes;
        for (size_t i = 0; i < data.size(); i++) {
            double minDist = -1.0;
            int closestIndex = -1;
            for (int j = 0; j < cb.size(); j++) {
                double d = euclidSquared(data.at(i), cb.at(j));
                if (j == 0 || d < minDist) {
                    minDist = d;
                    closest.at(i) = cb.at(j);
                    closestIndex = j;
                }
            }
            if (nearestVectors.find(closestIndex) == nearestVectors.end()) {
                nearestVectors[closestIndex] = vector<double*>();
            }
            if (nearestVectorsIndexes.find(closestIndex) == nearestVectorsIndexes.end()) {
                nearestVectorsIndexes[closestIndex] = vector<int>();
            }
            nearestVectors.at(closestIndex).push_back(data.at(i));
            nearestVectorsIndexes.at(closestIndex).push_back(i);
        }

        for (int i = 0; i < cb.size(); i++) {
            vector<double*> nearestVectorsOfI = nearestVectors.at(i);
            if (nearestVectorsOfI.size() > 0) {
                double* averageVector = avgVectorOfVectors(nearestVectorsOfI);
                cb.at(i) = averageVector;
                // for (auto& it: nearestVectorsIndexes.at(i)) {
                //     closest.at(it) = averageVector;
                // }
                for (int j = 0; j < nearestVectorsIndexes.at(i).size(); j++) {
                    closest.at(nearestVectorsIndexes.at(i).at(j)) = averageVector;
                }
            }
        }

        double prevAvgDist = averageDistortion > 0.0 ? averageDistortion : initialAvgDist;
        averageDistortion = avgDistortion(closest, data, dataSize);

        error = (prevAvgDist - averageDistortion) / prevAvgDist;
    }
    x = averageDistortion;
    return cb;
}

double* Quantizer::newVector(double vector[], double epsilon) {
    double* newVector = new double[3];
    for (size_t i = 0; i < 3; i++) {
        newVector[i] = vector[i] * (1.0 + epsilon);
    }
    return newVector;
}

double* Quantizer::getPixelAsDoubleArray(Pixel* a) {
    double* x = new double[3];
    x[0] = (double)(a->B());
    x[1] = (double)(a->G());
    x[2] = (double)(a->R());
    return x;
}
