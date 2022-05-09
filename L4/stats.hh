#ifndef __STATS_H
#define __STATS_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;
// typedef unsigned char uchar;
typedef uint8_t uchar;

void fillOccs (int* occs, int value);
int* initCharOccs (int value);
int* countCharOccs (uchar* array, int n);
int** countCharOccsCond (uchar* array, int n);

double entropy (int* charOccs, size_t n);
double entropyCond (int* charOccs, int** charOccsCond, size_t n);

double compressionRate (size_t fileSize, size_t codeSize);
double avgCodeLength (size_t fileSize, size_t codeSize);

#endif
