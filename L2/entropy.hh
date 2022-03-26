#ifndef __ENTROPY_H
#define __ENTROPY_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <tuple>
using namespace std;
typedef unsigned char uchar;

uchar* fileToArray (string filename);

int* initCharOccs (int value);
// tuple countChars(uchar* array, int n);
int* countCharOccs (uchar* array, int n);
int** countCharOccsCond (uchar* array, int n);

double entropy (int* charOccs);
double entropyCond (int** charOccsCond);

#endif