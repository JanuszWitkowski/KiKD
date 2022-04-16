#include <iostream>
#include <string.h>
#include "bitrw.hh"
#include "universal.hh"
#include "lzw.hh"
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

int main (int argc, char* argv[]) {
    if (argc >= 2) {
        string filename = "testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt";
        string codename = "output/kikd.kkd";
        UniversalCodingType type = OMEGA;
        for (size_t i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-f") == 0 && ++i < argc)
                filename = argv[i];
            else if (strcmp(argv[i], "-u") == 0 && ++i < argc) {
                if (strcmp(argv[i], "gamma") == 0) type = GAMMA;
                else if (strcmp(argv[i], "delta") == 0) type = DELTA;
                else if (strcmp(argv[i], "fib") == 0) type = FIBONACCI;
                else type = OMEGA;
            }
        }
        string newname = "output/" + filename;

        LZW_encode(filename, codename, type);
        LZW_decode(codename, newname);
    }

    uint n = 137;
    string g = eliasGamma(n);
    string d = eliasDelta(n);
    string o = eliasOmega(n);
    string f = fibonacci(n);
    uint ng = eliasGamma(g);
    uint nd = eliasDelta(d);
    uint no = eliasOmega(o);
    uint nf = fibonacci(f);
    cout << n << "\n";
    cout << g << " == " << ng << "\n";
    cout << d << " == " << nd << "\n";
    cout << o << " == " << no << "\n";
    cout << f << " == " << nf << "\n";

    string eliasname = "output/elias.txt";
    string inname = "testy/test1.bin";
    string kkdname = "output/test1.kkd";
    string outname = "output/test1.bin";

    uint tab[] = {137, 1, 0, 1234567890, 654};
    size_t tabSize = 5;
    BitWriter *writer = new BitWriter(eliasname);
    for (size_t i = 0; i < tabSize; i++) {
        cout << tab[i] << " ";
        eliasGamma(tab[i], writer);
        cout << tab[i] << " ";
        eliasDelta(tab[i], writer);
        cout << tab[i] << " ";
        eliasOmega(tab[i], writer);
        cout << tab[i] << " ";
        fibonacci(tab[i], writer);
    }
    cout << endl;
    writer->padWithZeros();
    delete writer;
    BitReader *reader = new BitReader(eliasname);
    for (size_t i = 0; i < tabSize; i++) {
        cout << eliasGamma(reader) << " ";
        cout << eliasDelta(reader) << " ";
        cout << eliasOmega(reader) << " ";
        cout << fibonacci(reader) << " ";
    }
    cout << endl;
    delete reader;

    return 0;
}
