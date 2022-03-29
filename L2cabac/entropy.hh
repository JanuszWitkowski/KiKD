#ifndef __ENTROPY_H
#define __ENTROPY_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;
typedef unsigned char uchar;

uchar* fileToArray (string filename, int &n);

void fillOccs (int* occs, int value);
int* initCharOccs (int value);
int* countCharOccs (uchar* array, int n);
int** countCharOccsCond (uchar* array, int n);

double entropy (int* charOccs);
double entropyCond (int** charOccsCond);

#endif