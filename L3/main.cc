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
    else {
        cout << "Sposob wywolania: " << endl;
        cout << "./main -f <nazwa_pliku> [-u gamma/delta/omega/fib]" << endl;
    }

    cout << "Sukces!" << endl;
    return 0;
}
