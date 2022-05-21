#include <iostream>
#include "pixel.hh"
#include "bitrw.hh"
#include "tga.hh"

using namespace std;

int main (void) {
    string filename = "testy/example1.tga";
    size_t n;
    uchar* file = fileToArray(filename, n);
    SimpleTGA* tga = new SimpleTGA(file, n);
    PixelBitmap* BGR = new PixelBitmap(tga->getBitmap(), tga->imageWidth, tga->imageHeight);

    tga->printSimpleTGA();
    string tganame = "test.tga";
    buildImageTGA(tganame, tga->getBitmap(), tga->getBitmapSize(), tga->imageWidth, tga->imageHeight);

    delete BGR;
    delete tga;
    delete[] file;
    return 0;
}
