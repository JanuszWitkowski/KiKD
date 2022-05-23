#include <iostream>
#include <chrono>
#include "pixel.hh"
#include "bitrw.hh"
#include "tga.hh"
#include "stats.hh"
#include "quantizer.hh"
#include "console_colors.hh"

using namespace std;

int integerPower (int e) {
    if (e == 0) return 1;
    return 2 * integerPower(e-1);
}

int main (int argc, char* argv[]) {
    if (argc != 4) {
        cout << cRed << "BŁĄD: Zła liczba argumentów." << endl;
        cout << "Wywołanie programu powinno wyglądać następująco:" << endl;
        cout << "./main [nazwa pliku wejściowego] [nazwa pliku wyjściowego] [wykładnik liczby kolorów 0-24]" << cReset << endl;
    } else {
        string inName = argv[1], outName = argv[2];
        int colorsNumberExp = stoi(argv[3]);
        if  (colorsNumberExp < 0 || colorsNumberExp > 24) {
            cout << cRed << "BŁĄD: Niepoprawny parametr liczby kolorów." << endl;
            cout << "Dany parametr powinien przyjąć liczbę całkowitą z zakresu [0, 24]." << cReset << endl;
        } else {
            size_t n;
            uchar* inFile = fileToArray(inName, n), *outFile;
            int colorsNumber = integerPower(colorsNumberExp);
            Quantizer* quantizer = new Quantizer(inFile, n, colorsNumber);
            auto start = chrono::steady_clock::now();
            outFile = quantizer->encode();
            auto end = chrono::steady_clock::now();
            double MSError = quantizer->mse();
            double SNR = quantizer->snr(MSError);
            cout << cBlue << "-------[KWANTYZACJA WEKTOROWA PLIKU " << inName << "]-------" << endl;
            cout << "Czas kwantyzacji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
            cout << "Czas kwantyzacji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
            cout << "Błąd średniokwadratowy: " << MSError << endl;
            cout << "Stosunek sygnału do szumu: " << SNR << endl;
            cout << "--------------------------------------------------------------" << cReset << endl;
            delete quantizer;
            delete[] inFile, outFile;
        }
    }
    return 0;
}
