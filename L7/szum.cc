#include <iostream>
#include <random>
#include <ctime>
#include "bitrw.hh"
#include "consts.hh"

using namespace std;

size_t szum(double p, string in, string out) {
    BitReader reader(in);
    BitWriter writer(out);
    size_t fileSize = reader.getFileSizeInBits();
    size_t counter = 0;
    for (size_t i = 0; i < fileSize; i++) {
        uint b = reader.getNextBit();
        if (((double) rand() / (RAND_MAX)) < p) {
            writer.writeBit(1-b);
            counter++;
        }
        else writer.writeBit(b);
    }
    return counter;
}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    cout << cGreen << "-----SZUM-----" << cReset << endl;
    if (argc != 4) {
        printError(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<prawdopodobieństwo odrócenia bitu [0, 1]> <nazwa pliku wejściowego> <nazwa pliku wyjściowego>");
        return 1;
    }
    double p = atof(argv[1]);
    if (p < 0.0 || p > 1.0) {
        printError(ARGUMENT, "Podane prawdopodobieństwo nie mieści się w zakresie [0, 1].");
        return 1;
    }
    string in = argv[2], out = argv[3];
    cout <<cGreen << "PODANE PARAMETRY\nPrawdopodobieństwo: " << cYellow << p << cGreen << "\nInput: " << cYellow << in << cGreen << "\nOutput: " << cYellow << out << cReset << endl;
    size_t counter = szum(p, in, out);
    cout << cBlue << "Nałożono faulty na " << counter << " bitów pliku." << cReset << endl;
    return 0;
}
