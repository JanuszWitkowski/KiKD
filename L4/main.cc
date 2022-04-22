#include <iostream>
#include "bitrw.hh"
#include "stats.hh"

using namespace std;

int main (int argc, char* argv[]) {
    size_t n;
    string filename = "pliki/small_sample_640x426.tga";
    uchar* array = fileToArray(filename, n);
    int* occs = countCharOccs(array, n);
    cout <<"Entropia pliku " << filename << endl;
    cout << entropy(occs, n) << endl;

    delete[] array;
    delete[] occs;
    return 0;
}
