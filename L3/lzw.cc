#include <iostream>
#include <chrono>
#include "lzw.hh"

void encode (string filename, string codename, UniversalCodingType type) {
    void (*universal)(uint, BitWriter*);
    switch (type) {
        case GAMMA:
        universal = eliasGamma;
        break;

        case DELTA:
        universal = eliasDelta;
        break;

        case OMEGA:
        universal = eliasOmega;
        break;

        case FIBONACCI:
        universal = fibonacci;
        break;

        default:
        universal = eliasOmega;
    }

    BitReader* reader = new BitReader(filename);
    BitWriter* writer = new BitWriter(codename);

    cout << "KOMPRESJA LZW " << endl;
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;

    delete reader;
    delete writer;
}

void decode (string codename, string filename) {
    //
}
