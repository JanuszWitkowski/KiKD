#include <iostream>
#include "bitrw.hh"
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

int main () {
    string filename, newname;

    uint n = 654;
    string g = eliasGamma(n);
    string d = eliasDelta(n);
    string o = eliasOmega(n);
    uint ng = eliasGamma(g);
    uint nd = eliasDelta(d);
    uint no = eliasOmega(o);
    cout << n << "\n";
    cout << g << " == " << ng << "\n";
    cout << d << " == " << nd << "\n";
    cout << o << " == " << no << "\n";

    filename = "testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt";
    newname = "output/test0.txt";
    // filename = "testy/test2.bin";
    // newname = "output/test2.bin";

    BitReader *reader = new BitReader(filename);
    reader->printArray();
    BitWriter *writer = new BitWriter(newname);
    int bit = 0;
    for (size_t i = 0; i < reader->getFileSize() * 8; i++) {
        bit = reader->isNextBitOne() ? 1:0;
        writer->writeBit(bit);
    }
    delete(reader);
    delete(writer);

    return 0;
}
