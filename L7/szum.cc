#include <iostream>
#include <memory>
#include <random>
#include <ctime>
#include "bitrw.hh"
#include "consts.hh"

using namespace std;

void szum(double p, string in, string out) {
    // unique_ptr<BitReader> reader = make_unique<BitReader>(in);
    BitReader reader(in);
    BitWriter writer(out);
    size_t fileSize = reader.getFileSize();
    size_t counter = 0;
    for (size_t i = 0; i < fileSize*8; i++) {
        uint b = reader.getNextBit();
        // double r = ((double) rand() / (RAND_MAX));
        if (((double) rand() / (RAND_MAX)) < p) {
            writer.writeBit(1-b);
            counter++;
        }
        else writer.writeBit(b);
    }
    cout << "counter = " << counter << endl;
}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    if (argc != 4) {
        // cerr << "ARGUMENT ERROR: Niepoprawna liczba argumentów. Schemat argumentów:\n";
        // cerr << "<prawdopodobieństwo odrócenia bitu [0, 1]> <nazwa pliku wejściowego> <nazwa pliku wyjściowego>" << endl;
        errorMsg(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<prawdopodobieństwo odrócenia bitu [0, 1]> <nazwa pliku wejściowego> <nazwa pliku wyjściowego>");
        return 1;
    }
    // double p = static_cast<double> (argv[1]);
    double p = atof(argv[1]);
    if (p < 0.0 || p > 1.0) {
        // cerr << "ARGUMENT ERROR: Podane prawdopodobieństwo nie mieści się w zakresie [0, 1]." << endl;
        errorMsg(ARGUMENT, "Podane prawdopodobieństwo nie mieści się w zakresie [0, 1].");
        return 1;
    }
    string in = argv[2], out = argv[3];
    cout << "PODANE PARAMETRY\nPrawdopodobieństwo: " << p << "\nInput: " << in << "\nOutput: " << out << endl;
    szum(p, in, out);
    return 0;
}
