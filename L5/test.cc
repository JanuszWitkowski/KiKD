#include <iostream>
#include "pixel.hh"
#include "bitrw.hh"
#include "tga.hh"
#include "quantizer.hh"

using namespace std;

void testBlue () {
    string filename = "colors/blue.tga";
    size_t n;
    uchar* file = fileToArray(filename, n);
    SimpleTGA* tga = new SimpleTGA(file, n);
    tga->printSimpleTGA();

    delete[] file;
    delete tga;
}

int main (void) {
    // testBlue();
    string filename = "testy/example1.tga";
    string gebalaname = "testy/example0.tga";
    size_t n, m;
    uchar* file = fileToArray(filename, n);
    uchar* gebala = fileToArray(gebalaname, m);
    TGAHeader* header = new TGAHeader(gebala);
    TGAFooter* footer = new TGAFooter(gebala, m);
    cout << "Gębala Size: " << m << endl;
    header->printTGAHeader();
    footer->printTGAFooter();

    SimpleTGA* GebalaTGA = new SimpleTGA(gebalaname, gebala, m);
    GebalaTGA->printSimpleTGA();
    SimpleTGA* tga = new SimpleTGA(file, n);
    // PixelBitmap* BGR = new PixelBitmap(tga->getBitmap(), tga->imageWidth, tga->imageHeight);

    // printArrayToFile("output/array.txt", tga->getBitmap(), tga->imageWidth, tga->imageHeight);
    printArrayToFile("output/array.txt", GebalaTGA->getBitmap(), GebalaTGA->imageWidth, GebalaTGA->imageHeight);

    // Quantizer* q = new Quantizer(file, n, 2*2*2);
    // uchar* output = q->encode("output/test.tga");

    // delete BGR;
    delete tga;
    delete header;
    delete footer;
    delete[] file;
    // delete q;
    return 0;
}
