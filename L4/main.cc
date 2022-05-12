#include <iostream>
#include "bitrw.hh"
#include "stats.hh"
#include "tga.hh"
#include "jpeg-ls.hh"

using namespace std;

int main (int argc, char* argv[]) {
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
        // cout << bitmapSize/3 << endl;
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
        double entropies[9][4];
        for (size_t i = 0; i < 9; i++) {
            size_t colorSize = bitmapSize/3;
            int** occs = new int*[4];
            for (size_t j = 0; j < 3; j++) {
                occs[j] = countCharOccs(predCodes[i][j], colorSize);
                entropies[i][j] = entropy(occs[j], colorSize);
            }
            // uint8_t* newBitmap = codesToBitmap(predCodes[i], colorSize);
            // occs[4] = countCharOccs(newBitmap, colorSize*3);
            // entropies[i][3] = entropy(occs[4], colorSize*3);
            for (size_t j = 0; j < 4; j++) {
                delete[] occs[j];
            }
            delete[] occs;
        }

        for (size_t i = 0; i < 9; i++) {
            cout << "PREDYKTOR " << i << endl;
            cout << "Entropia BLUE: " << entropies[i][BLUE] << endl;
            cout << "Entropia GREEN: " << entropies[i][GREEN] << endl;
            cout << "Entropia RED: " << entropies[i][RED] << endl;
            cout << endl;
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
    return 0;
}
