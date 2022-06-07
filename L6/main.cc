#include <iostream>
#include <chrono>
#include "main.hh"
#include "consts.hh"
#include "differential.hh"
#include "tga.hh"

using namespace std;

void printError(string errorType, string errorMsg) {
    std::cout << cRed << errorType << " ERROR: ";
    std::cout << errorMsg << cReset << std::endl;
}

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
    // SimpleTGA* tga = new SimpleTGA(inName, array, n);
    SimpleTGA* tga = new SimpleTGA(array, n);
    PixelArray* pixels = tga->getPixelBitmap()->colorsArray(tga->imageWidth, tga->imageHeight);
    uchar** colors = pixels->pixelarrayToColorsArray();
    size_t length = pixels->getSize();

    auto start = chrono::steady_clock::now();
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
    auto end  = chrono::steady_clock::now();
    printBandsToFile(outName, codings[0], codings[1], tga->imageWidth, tga->imageHeight, qBits);

    cout << cBlue << "-----KWANTYZACJA RÓWNOMIERNA Z ROZBICIEM NA DWA FILTRY-----" << endl;
    cout << inName << " -> " << outName << endl;
    cout << "Liczba bitów kwantyzatora: " << qBits << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "-----------------------------------------------------------" << cReset << endl;

    for (size_t color = 0; color < 3; color++)
        delete[] colors[color];
    delete[] colors;
    delete pixels;
    delete tga;
    delete[] array;
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
}

void decodeTGA(string inName, string outName) {
    auto start = chrono::steady_clock::now();
    BandSolver* bands = new BandSolver(inName);
    auto end = chrono::steady_clock::now();
    // cout << bands->getWidth() << " " << bands->getHeight() << " " << inName << " " << outName << endl;
    printArrayToFile("output/tmp.txt", outName, bands->getBitmap(), bands->getWidth(), bands->getHeight());

    cout << cCyan << "-----------DEKOMPRESJA PLIKU DO TGA-----------" << endl;
    cout << inName << " -> " << outName << endl;
    cout << "Liczba bitów kwantyzatora: " << bands->getBits() << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "----------------------------------------------" << cReset << endl;

    delete bands;
}

void compareImages(std::string filename1, std::string filename2) {
    // size_t n1, n2;
    // uchar *array1 = fileToArray(filename1, n1), *array2 = fileToArray(filename2, n2);
    // SimpleTGA *original = new SimpleTGA(filename1, array1, n1);
    // PixelBitmap *impostorBitmap = new PixelBitmap(array2, )
    // double MSE = mse(original, impostor);
    // double SNR = snr(original, MSE);

    // cout << cGreen << "----------------PODSUMOWANIE----------------" << endl;
    // cout << filename1 << " vs " << filename2 << endl;
    // cout << "MSE: " << MSE << endl;
    // cout << "SNR: " << SNR << endl;
    // cout << "--------------------------------------------" << cReset << endl;

    // delete[] array1;
    // delete[] array2;
    // delete original;
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
