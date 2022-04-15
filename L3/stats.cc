#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

#include "stats.hh"
#include "bitrw.hh"
using namespace std;
typedef unsigned char uchar;
// typedef uint8_t uchar;

void fillOccs (int* occs, int value) {
    for (int i = 0; i < 256; i++) {
        occs[i] = value;
    }
}

int* initCharOccs (int value) {
    int* charOccs = new int[256];
    fillOccs(charOccs, value);
    return charOccs;
}

int* countCharOccs (uchar* array, int n) {
    int* charOccs = initCharOccs(0);
    for (int i = 0; i < n; i++) {
        charOccs[array[i]]++;
    }
    return charOccs;
}

int** countCharOccsCond (uchar* array, int n) {
    int** charOccsCond = new int*[256];
    for (int i = 0; i < 256; i++) {
        charOccsCond[i] = new int[256];
        for (int j = 0; j < 256; j++) {
            charOccsCond[i][j] = 0;
        }
    }
    int prev = 0;
    for (int i = 0; i < n; i++) {
        charOccsCond[array[i]][prev]++;
        prev = array[i];
    }
    return charOccsCond;
}

// na iteracje: 1 sprawdzenie, 1 logarytm, 1 mnozenie, 1 odejmowanie
double entropy (int* charOccs, size_t n) {
    double H = 0.0;
    double logOfN = log2(n);
    double p, i;
    for (size_t j = 0; j < 256; j++) {
        if (charOccs[j] != 0) {
            H += charOccs[j] * (logOfN - log2(charOccs[j]));
        }
    }
    return H / n;
}

double entropyCond (int* charOccs, int** charOccsCond, size_t n) {
    double H = 0.0;
    double HCond, logOfX;
    for (size_t j = 0; j < 256; j++) {   // 1 sprawdzenie, 1 logarytm, 1 konwersja, 1 dodawanie, 256 iteracji
        HCond = 0.0;
        if (charOccs[j] != 0) {
            logOfX = log2(1.0 * charOccs[j]);
            for (size_t i = 0; i < 256; i++) {   // 1 sprawdzenie, 1 logarytm, 1 odejmowanie, 1 mnozenie, 1 dodawanie
                if (charOccsCond[i][j] != 0) {
                    HCond += charOccsCond[i][j] * (logOfX - log2(charOccsCond[i][j]));
                }
            }
        }
        H += HCond;
    }
    return H / n;
}

double compressionRate (size_t fileSize, size_t codeSize) {
    double n = 1.0 * fileSize, m = 1.0 * codeSize;
    return 100.0 * (1.0 - (m / n));
}

double avgCodeLenght (size_t fileSize, size_t codeSize) {
    double n = 1.0 * fileSize, m = 1.0 * codeSize;
    return (8.0 * m) / n;
}

void summary (string filename, string codename) {
    uchar *file, *code;
    size_t fileSize, codeSize;
    double rate, fileEntropy, codeEntropy, avgLength;
    int *fileOccs, *codeOccs;
    
    file = fileToArray(filename, fileSize);
    code = fileToArray(codename, codeSize);
    fileOccs = countCharOccs(file, fileSize);
    codeOccs = countCharOccs(code, codeSize);
    rate = compressionRate(fileSize, codeSize);
    fileEntropy = entropy(fileOccs, fileSize);
    codeEntropy = entropy(codeOccs, codeSize);
    avgLength = avgCodeLenght(fileSize, codeSize);

    delete(file);
    delete(code);
    delete(fileOccs);
    delete(codeOccs);

    cout << "---------------PODSUMOWANIE---------------" << endl;
    cout << "Plik kodowany: " << filename << endl;
    cout << "Plik skompresowany: " << codename << endl;
    cout << "=========================================" << endl;
    cout << "Dlugosc pliku kodowanego: " << fileSize << endl;
    cout << "Dlugosc pliku po kompresji: " << codeSize << endl;
    cout << "Stopien kompresji: " << rate << endl;
    cout << "=========================================" << endl;
    cout << "Entropia pliku kodowanego: " << fileEntropy << endl;
    cout << "Entropia pliku po kompresji: " << codeEntropy << endl;
    cout << "Srednia dlugosc kodu: " << avgLength << endl;
    cout << "------------------------------------------" << endl;
}
