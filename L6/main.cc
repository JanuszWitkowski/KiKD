#include <iostream>
#include <cstring>
#include <chrono>
#include "main.hh"
#include "consts.hh"
#include "differential.hh"
#include "tga.hh"

using namespace std;

bool checkArguments(int c, char* v[], string &arg1, string &arg2, size_t &arg3) {
    if (c < 3 or c > 4) {
        printError("ARGUMENT", "Niepoprawna liczba argumentów.\nPrawidłowe wywołania programu:\n\
                    ./encode <plik wejściowy> <plik wyjściowy> <liczba bitów kwantyzatora [1, 7]>\n\
                    <plik wejściowy> <liczba bitów kwantyzatora [1, 7]>");
        return false;
    }
    string inName = "testy/example0.tga", outName = "output/kikd.kkd";
    size_t qBits = 3;
    inName = v[1];
    if (c == 3) {
        qBits = atof(v[2]);
    } else {
        outName = v[2];
        qBits = atof(v[3]);
    }
    if (qBits < 1 or qBits > 7) {
        printError("QUANTIZER", "Nieprawidłowa liczba bitów kwantyzatora (powinna być z zakesu {1, 2, ..., 7}).");
        return false;
    }
    arg1 = inName;
    arg2 = outName;
    arg3 = qBits;
    return true;
}

void encodeTGA(string inName, string outName, size_t qBits) {
    size_t n;
    uchar* array = fileToArray(inName, n);
    SimpleTGA* tga = new SimpleTGA(inName, array, n);
    PixelArray* pixels = tga->getPixelBitmap()->colorsArray();
    uchar** colors = pixels->pixelarrayToColorsArray();
    size_t length = pixels->getSize();
    FilterHolder *downs = new FilterHolder(length), *ups = new FilterHolder(length);
    BandSolver *bands = new BandSolver(length);

    auto start = chrono::steady_clock::now();
    for (size_t color = 0; color < 3; color++) {
        downs->setFilter(color, filterAverage(colors[color], length));
        ups->setFilter(color, filterDeviation(colors[color], length));
        bands->setCodingBand(0, color, differentialCoding(downs->getFilter()[color], length, qBits));
        bands->setCodingBand(1, color, straightQuantizing(ups->getFilter()[color], length, qBits));
    }
    auto end  = chrono::steady_clock::now();
    printBandsToFile(outName, bands->getCoding(0), bands->getCoding(1), tga->imageWidth, tga->imageHeight, qBits);

    cout << cBlue << "-----KWANTYZACJA RÓWNOMIERNA Z ROZBICIEM NA DWA FILTRY-----" << endl;
    cout << inName << " -> " << outName << endl;
    cout << "Liczba bitów kwantyzatora: " << qBits << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "-----------------------------------------------------------" << cReset << endl;

    delete downs, ups;
    for (size_t color = 0; color < 3; color++)
        delete[] colors[color];
    delete[] colors;
    delete bands;
    delete pixels;
    delete tga;
    delete[] array;
}

void decodeTGA(string inName, string outName) {
    //
    string strCreate = "python3 output/tmp.txt" + outName, strDelete = "rm output/tmp.txt";
    char cmdCreate[strCreate.size()], cmdDelete[strDelete.size()];
    strcpy(cmdCreate, strCreate.c_str());
    strcpy(cmdDelete, strDelete.c_str());
    // THIS BELOW IS CHEATING and I'm not sorry
    system(cmdCreate);
    system(cmdDelete);
}

void compareImages(std::string filename1, std::string filename2) {
    //
}


int main (int argc, char* argv[]) {
    string inName, outName, newName = "output/example.tga";
    size_t qBits;
    if (!checkArguments(argc, argv, inName, outName, qBits)) return 1;
    encodeTGA(inName, outName, qBits);
    decodeTGA(outName, newName);
    compareImages(inName, newName);
    return 0;
}
