#include <iostream>
#include "pixel.hh"
#include "bitrw.hh"
#include "tga.hh"
#include "quantizer.hh"

using namespace std;

int main (void) {
    string filename = "testy/example1.tga";
    size_t n;
    uchar* file = fileToArray(filename, n);
    SimpleTGA* tga = new SimpleTGA(file, n);
    // PixelBitmap* BGR = new PixelBitmap(tga->getBitmap(), tga->imageWidth, tga->imageHeight);

    printArrayToFile("output/array.txt", tga->getBitmap(), tga->imageWidth, tga->imageHeight);

    // Quantizer* q = new Quantizer(file, n, 2*2*2);
    // uchar* output = q->encode("output/test.tga");

    // delete BGR;
    delete tga;
    delete[] file;
    // delete q;
    return 0;
}
