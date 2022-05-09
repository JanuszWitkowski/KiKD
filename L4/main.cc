#include <iostream>
#include "bitrw.hh"
#include "stats.hh"
#include "tga.hh"

using namespace std;

int main (int argc, char* argv[]) {
    size_t n;
    string filename = "testy/example1.tga";
    uchar* array = fileToArray(filename, n);

    TGA *tga = new TGA(array, n);
    // tga->printTGA();
    // printArray(array, 18);
    // cout << endl;

    SimpleTGA *stga = new SimpleTGA(array, n);
    int* occs = countCharOccs(stga->getBitMap(), n);
    cout <<"Entropia mapy bitowej pliku " << filename << endl;
    cout << entropy(occs, n) << endl;
    cout << n << endl;
    cout << tga->getHeader()->getImageHeight() * tga->getHeader()->getImageWidth() << endl;
    cout << stga->getBitMapSize() << endl;
    stga->getHeader()->printTGAHeader();
    stga->getFooter()->printTGAFooter();
    uint8_t a = 1, b = 2, c = a-b;

    // for (size_t i = 0; i < stga->getBitMapSize(); i++) {
    //     if (tga->getImageID()[i] != stga->getBitMap()[i]) {
    //         cout << "Niezgoda dla i = " << i << endl;
    //         break;
    //     }
    // }

    delete tga;
    delete stga;
    delete[] array;
    delete[] occs;
    return 0;
}
