#ifndef __STATS_H
#define __STATS_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;
// typedef unsigned char uchar;
typedef uint8_t uchar;

void fillOccs (int occs[], int value);
int* initCharOccs (int value);
int* countCharOccs (const uchar array[], int n);
int* countCharOccsWithIncrement (const uchar array[], int n, size_t starting_point, size_t inc);
int** countCharOccsCond (const uchar array[], int n);

double entropy (const int charOccs[], size_t n);
double entropyCond (int charOccs[], const int** charOccsCond, size_t n);

double compressionRate (size_t fileSize, size_t codeSize);
double avgCodeLength (size_t fileSize, size_t codeSize);

#endif
