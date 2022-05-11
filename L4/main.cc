#include <iostream>
#include "bitrw.hh"
#include "stats.hh"
#include "tga.hh"
#include "jpeg-ls.hh"

using namespace std;

int main (int argc, char* argv[]) {
    // size_t n;
    // string filename = "testy/example1.tga";
    // uchar* array = fileToArray(filename, n);

    // TGA *tga = new TGA(array, n);
    // tga->printTGA();
    // printArray(array, 18);
    // cout << endl;

    // SimpleTGA *stga = new SimpleTGA(array, n);
    // int* occs = countCharOccs(stga->getBitMap(), n);
    // cout <<"Entropia mapy bitowej pliku " << filename << endl;
    // cout << entropy(occs, n) << endl;
    // cout << n << endl;
    // cout << stga->imageWidth << " " << stga->imageHeight << endl;
    // cout << tga->getHeader()->getImageHeight() * tga->getHeader()->getImageWidth() << endl;
    // cout << stga->getBitMapSize() << endl;
    // uint8_t a = 1, b = 2, c = a-b;

    if (argc < 2) return 1;
    string filename = argv[1];
    size_t n;
    uchar* array = fileToArray(filename, n);
    if (filename == "testy/example0.tga") {
        //
    }
    else {
        SimpleTGA* stga = new SimpleTGA(array, n);
        uint8_t* bitmap = stga->getBitMap();
        size_t bitmapSize = stga->getBitMapSize();
        size_t width = stga->imageWidth;
        size_t height = stga->imageHeight;
        // cout <<"!!" << bitmapSize << "-!!" << width << "-!!" << height << endl;
        uint8_t*** BGR = bitmapToBGR(bitmap, bitmapSize, width, height);

        uint8_t*** predCodes = new uint8_t**[9];
        predCodes[0] = BGRToPredictionCodes(BGR, width, height, pred0);
        predCodes[1] = BGRToPredictionCodes(BGR, width, height, pred1);
        predCodes[2] = BGRToPredictionCodes(BGR, width, height, pred2);
        predCodes[3] = BGRToPredictionCodes(BGR, width, height, pred3);
        predCodes[4] = BGRToPredictionCodes(BGR, width, height, pred4);
        predCodes[5] = BGRToPredictionCodes(BGR, width, height, pred5);
        predCodes[6] = BGRToPredictionCodes(BGR, width, height, pred6);
        predCodes[7] = BGRToPredictionCodes(BGR, width, height, pred7);
        predCodes[8] = BGRToPredictionCodes(BGR, width, height, pred8);
        // cout << "!!" << predCodes[0][0] << endl;
        size_t entropies[9][4];
        for (size_t i = 0; i < 9; i++) {
            int* occsBlue = countCharOccsWithIncrement(predCodes[i][BLUE], bitmapSize, BLUE, 3);
            entropies[i][BLUE] = entropy(occsBlue, bitmapSize);
            delete[] occsBlue;
        }

        for (size_t i = 0; i < 9; i++) {
            cout << "PREDYKTOR " << i << endl;
            cout << "Entropia BLUE: " << entropies[i][BLUE] << endl;
        }

        delete stga;
        for (size_t i = 0; i < 9; i++) {
            for (size_t j = 0; j < 3; j++) {
                delete[] predCodes[i][j];
            }
            delete[] predCodes[i];
        }
        delete[] predCodes;
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                delete[] BGR[i][j];
            }
            delete[] BGR[i];
        }
        delete[] BGR;
    }

    delete[] array;

    // for (size_t i = 0; i < stga->getBitMapSize(); i++) {
    //     if (tga->getImageID()[i] != stga->getBitMap()[i]) {
    //         cout << "Niezgoda dla i = " << i << endl;
    //         break;
    //     }
    // }

    // delete tga;
    // delete stga;
    // delete[] array;
    // delete[] occs;
    return 0;
}
