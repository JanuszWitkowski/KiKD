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
  int **charOccsCond;   // wystapienia poszczegoknych znakow po danych znakach
public:
  File (string filename) {
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

  virtual ~File (void) {
    delete array;
    delete charOccs;
    delete charOccsCond;
  }

  uchar byte (int i) {
    return array[i];
  }

  void byte (int i, uchar c) {
    array[i] = c;
  }

  int length (void) {
    return n;
  }

  void countChars (void) {
      int prev = 0;
      for (int i = 0; i < n; i++) {
          charOccs[array[i]]++;
          charOccsCond[array[i]][prev]++;
          prev = array[i];
      }
  }

  double entropy (void) {
    double H = 0.0;
    double p, i;
    for (int j = 0; j < 256; j++) {
      if (charOccs[j] != 0) {
        p = 1.0 * charOccs[j] / n;
        i = -log2(p);
        H += p * i;
      }
    }
    return H;
  }

  double entropyCond (void) {
    double H = 0.0;
    double HCond, logOfX;
    for (int j = 0; j < 256; j++) {
      HCond = 0.0;
      if (charOccs[j] != 0) {
        logOfX = log2(1.0 * charOccs[j]);
        for (int i = 0; i < 256; i++) {
          if (charOccsCond[i][j] != 0) {
            HCond += charOccsCond[i][j] * (logOfX - log2(charOccsCond[i][j]));
          }
        }
      }
      H += HCond / n;
    }
    return H;
  }

  void printArray (void) {
      for (int i = 0; i < n; i++) {
          cout << array[i] << " ";
      }
  }

  void printBytesOccurances (void) {
      for (int i = 0; i < 256; i++) {
          cout << i << ": " << charOccs[i] << endl;
      }
  }

  void printCharacterOccurances (void) {
      for (int i = 0; i < 256; i++) {
          cout << (char)i << " " << charOccs[i] << endl;
      }
  }

  void printBytesCondOccurances (void) {
      for (int i = 0; i < 256; i++) {
          for (int j = 0; j < 256; j++) {
            cout << charOccsCond[i][j] << " ";
          }
          cout << endl;
      }
  }
};


int main (int argc, char** argv) {
  if (argc < 2) return 1;
  string filename = argv[1];
  File file(filename);
  double entropy = file.entropy();
  double entropyCond = file.entropyCond();
  cout << "Entropy: " << entropy << endl;
  cout << "Conditional Entropy: " << entropyCond << endl;
  cout << "DIFFERENCE: " << entropy - entropyCond << endl;
  return 0;
}
