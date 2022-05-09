#include <iostream>
#include "bitrw.hh"
#include "stats.hh"
#include "tga.hh"

using namespace std;

int main (int argc, char* argv[]) {
    size_t n;
    string filename = "testy/example0.tga";
    uchar* array = fileToArray(filename, n);
    int* occs = countCharOccs(array, n);
    cout <<"Entropia pliku " << filename << endl;
    cout << entropy(occs, n) << endl;

    // TGA *tga = new TGA(array, n);
    // tga->printTGA();
    // printArray(array, 18);
    // cout << endl;

    SimpleTGA *stga = new SimpleTGA(array, n);
    // stga->printSimpleTGA();
    stga->getHeader()->printTGAHeader();
    stga->getFooter()->printTGAFooter();

    delete[] array;
    delete[] occs;
    return 0;
}
