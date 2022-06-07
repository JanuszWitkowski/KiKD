#include <iostream>
#include "bitrw.hh"
#include "tga.hh"
#include "differential.hh"

using namespace std;

int main (void) {
    cout << "TEST" << endl;
    string filename = "testy/example0.tga";
    string codename = "output/kikd.kkd";
    string newname = "output/example0.txt";
    size_t n;
    size_t qBits = 7;
    uchar* array = fileToArray(filename, n);
    SimpleTGA* tga = new SimpleTGA(filename, array, n);
    PixelArray* pixels = tga->getPixelBitmap()->colorsArray();
    uchar** colors = pixels->pixelarrayToColorsArray();
    size_t length = pixels->getSize();

    double*** filters = new double**[2];
    filters[0] = new double*[3];
    filters[1] = new double*[3];
    uchar*** codings = new uchar**[2];
    codings[0] = new uchar*[3];
    codings[1] = new uchar*[3];
    for (size_t i = 0; i < 3; i++) {
        filters[0][i] = filterAverage(colors[i], length);
        filters[1][i] = filterDeviation(colors[i], length);
        codings[0][i] = differentialCoding(filters[0][i], length, qBits);
        codings[1][i] = straightQuantizing(filters[1][i], length, qBits);
    }

    printBandsToFile(codename, codings[0], codings[1], tga->imageWidth, tga->imageHeight, qBits);
    BandSolver* bands = new BandSolver(codename);
    printArrayToFile(newname, bands->getBitmap(), tga->imageWidth, tga->imageHeight);

    // CLEAN-UP
    for (size_t i = 0; i < 3; i++) {
        delete[] filters[0][i];
        delete[] filters[1][i];
        delete[] codings[0][i];
        delete[] codings[1][i];
    }
    delete[] filters[0];
    delete[] filters[1];
    delete[] codings[0];
    delete[] codings[1];
    delete[] filters;
    delete[] codings;
    delete bands;
    delete[] colors[BLUE];
    delete[] colors[GREEN];
    delete[] colors[RED];
    delete[] colors;
    delete pixels;
    delete tga;
    delete[] array;
    cout << "Koniec garbage collectora." << endl;

    return 0;
}
