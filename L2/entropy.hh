#ifndef __ENTROPY_H
#define __ENTROPY_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;
typedef unsigned char uchar;

class File {
private:
  uchar *array;     // bajty
  int n;            // dlugosc
  int *charOccs;        // wystapienia poszczegolnych znakow
  int **charOccsCond;   // wystapienia poszczegolnych znakow po danych znakach
public:
  File (string filename);

  virtual ~File (void);

  void countChars (void);

  double entropy (void);
  double entropyCond (void);

  void printArray (void);
  void printBytesOccurances (void);
  void printCharacterOccurances (void);
  void printBytesCondOccurances (void);

  uchar byte (int i) {
    return array[i];
  }

  void byte (int i, uchar c) {
    array[i] = c;
  }

  int length (void) {
    return n;
  }
};

#endif