#include <iostream>
#include "bitrw.hh"
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

int main () {
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

    BitReader *reader = new BitReader("testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt");
    BitWriter *writer = new BitWriter("output/test0.txt");
    int bit = 0;
    for (int i = 0; i < reader->getFileSize(); i++) {
        bit = reader->isNextBitOne() ? 1:0;
        writer->writeBit(bit);
    }
    delete(reader);
    delete(writer);

    return 0;
}
