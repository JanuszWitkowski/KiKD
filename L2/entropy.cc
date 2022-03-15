#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

#include "entropy.hh"
using namespace std;
typedef unsigned char uchar;

File::File (string filename) {
    ifstream fin;
    fin.open(filename, ios::in|ios::binary);
    streampos start = fin.tellg();
    fin.ignore(numeric_limits<streamsize>::max());
    n = fin.gcount();
    fin.seekg(start,ios_base::beg);
    array = new uchar[n];
    fin.read((char *)array, n);
    fin.close();

    charOccs = new int[256];
    for (int i = 0; i < 256; i++) {
        charOccs[i] = 0;
    }
    charOccsCond = new int*[256];
    for (int i = 0; i < 256; i++) {
        charOccsCond[i] = new int[256];
        for (int j = 0; j < 256; j++) {
            charOccsCond[i][j] = 0;
        }
    }
    countChars();
}

File::~File (void) {
    delete array;
    delete charOccs;
    delete charOccsCond;
}

void File::countChars (void) {
      int prev = 0;
      for (int i = 0; i < n; i++) {
          charOccs[array[i]]++;
          charOccsCond[array[i]][prev]++;
          prev = array[i];
      }
}

// na iteracje: 1 sprawdzenie, 1 logarytm, 1 mnozenie, 1 odejmowanie
double File::entropy (void) {
    double H = 0.0;
    double logOfN = log2(n);
    double p, i;
    for (int j = 0; j < 256; j++) {
      if (charOccs[j] != 0) {
        H += charOccs[j] * (logOfN - log2(charOccs[j]));
      }
    }
    return H / n;
}

double File::entropyCond (void) {
    double H = 0.0;
    double HCond, logOfX;
    for (int j = 0; j < 256; j++) {   // 1 sprawdzenie, 1 logarytm, 1 konwersja, 1 dodawanie, 256 iteracji
      HCond = 0.0;
      if (charOccs[j] != 0) {
        logOfX = log2(1.0 * charOccs[j]);
        for (int i = 0; i < 256; i++) {   // 1 sprawdzenie, 1 logarytm, 1 odejmowanie, 1 mnozenie, 1 dodawanie
          if (charOccsCond[i][j] != 0) {
            HCond += charOccsCond[i][j] * (logOfX - log2(charOccsCond[i][j]));
          }
        }
      }
      H += HCond;
    }
    return H / n;
}

void File::printArray (void) {
      for (int i = 0; i < n; i++) {
          cout << array[i] << " ";
      }
}

void File::printBytesOccurances (void) {
      for (int i = 0; i < 256; i++) {
          cout << i << ": " << charOccs[i] << endl;
      }
}

void File::printCharacterOccurances (void) {
      for (int i = 0; i < 256; i++) {
          cout << (char)i << " " << charOccs[i] << endl;
      }
}

void File::printBytesCondOccurances (void) {
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      cout << charOccsCond[i][j] << " ";
    }
    cout << endl;
  }
}
