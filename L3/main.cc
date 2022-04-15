#include <iostream>
#include "bitrw.hh"
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

int main () {
    // string filename, newname;

    uint n = 137;
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

    // filename = "testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt";
    // newname = "output/test0.txt";
    // filename = "testy/test2.bin";
    // newname = "output/test2.bin";
    string outputname = "output/gamma.txt";

    // BitReader *reader = new BitReader(filename);
    // BitWriter *writer = new BitWriter(newname);
    // int bit = 0;
    // for (size_t i = 0; i < reader->getFileSize() * 8; i++) {
    //     bit = reader->isNextBitOne() ? 1:0;
    //     writer->writeBit(bit);
    // }
    // delete(reader);
    // delete(writer);

    uint tab[] = {137, 1, 0, 1234567890, 654};
    size_t tabSize = 5;
    BitWriter *writer = new BitWriter(outputname);
    for (size_t i = 0; i < tabSize; i++) {
        cout << tab[i] << " ";
        eliasGamma(tab[i], writer);
        cout << tab[i] << " ";
        eliasDelta(tab[i], writer);
        cout << tab[i] << " ";
        eliasOmega(tab[i], writer);
    }
    cout << endl;
    writer->padWithZeros();
    delete writer;
    BitReader *reader = new BitReader(outputname);
    for (size_t i = 0; i < tabSize; i++) {
        cout << eliasGamma(reader) << " ";
        cout << eliasDelta(reader) << " ";
        cout << eliasOmega(reader) << " ";
    }
    cout << endl;
    delete reader;

    return 0;
}
