#include <iostream>
#include <vector>
#include <chrono>
#include "lzw.hh"
#include "stats.hh"

void encode (string filename, string codename, UniversalCodingType type) {
    BitReader* reader = new BitReader(filename);
    BitWriter* writer = new BitWriter(codename);

    void (*universal)(uint, BitWriter*);
    uint bit0, bit1;
    switch (type) {
        case GAMMA:
        universal = eliasGamma;
        bit0 = 0; bit1 = 0;
        break;

        case DELTA:
        universal = eliasDelta;
        bit0 = 0; bit1 = 1;
        break;

        case OMEGA:
        universal = eliasOmega;
        bit0 = 1; bit1 = 0;
        break;

        case FIBONACCI:
        universal = fibonacci;
        bit0 = 1; bit1 = 1;
        break;

        default:
        universal = eliasOmega;
        bit0 = 1; bit1 = 0;
    }
    writer->writeBit(bit0);
    writer->writeBit(bit1);
    size_t fileSize = reader->getFileSize();
    (*universal)(fileSize, writer);

    vector<uchar*> dict;
    for (size_t i = 0; i < 256; i++) {
        dict.push_back(i);
    }

    writer->padWithZeros();
    delete reader;
    delete writer;
}

void decode (string codename, string filename) {
    BitReader* reader = new BitReader(codename);
    BitWriter* writer = new BitWriter(filename);

    uint (*universal)(BitReader*);
    uint bit0 = reader->getNextBit();
    uint bit1 = reader->getNextBit();
    if (bit0 == 0) {
        if (bit1 == 0) universal = eliasGamma;
        else universal = eliasDelta;
    }
    else {
        if (bit1 == 0) universal = eliasOmega;
        else universal = fibonacci;
    }
    size_t fileSize = (*universal)(reader);

    //

    delete reader;
    delete writer;
}


void LZW_encode (string filename, string codename, UniversalCodingType type) {
    string universal_code_name;
    switch (type) {
        case GAMMA:
        universal_code_name = "Elias Gamma";
        break;

        case DELTA:
        universal_code_name = "Elias Delta";
        break;

        case OMEGA:
        universal_code_name = "Elias Omega";
        break;

        case FIBONACCI:
        universal_code_name = "Fibonacci";
        break;

        default:
        universal_code_name = "Elias Omega (default)";
    }

    auto start = chrono::steady_clock::now();
    encode(filename, codename, type);
    auto end = chrono::steady_clock::now();

    uchar *file, *code;
    size_t fileSize, codeSize;
    double rate, fileEntropy, codeEntropy, avgLength;
    int *fileOccs, *codeOccs;
    
    file = fileToArray(filename, fileSize);
    code = fileToArray(codename, codeSize);
    fileOccs = countCharOccs(file, fileSize);
    codeOccs = countCharOccs(code, codeSize);
    rate = compressionRate(fileSize, codeSize);
    fileEntropy = entropy(fileOccs, fileSize);
    codeEntropy = entropy(codeOccs, codeSize);
    avgLength = avgCodeLength(fileSize, codeSize);

    delete(file);
    delete(code);
    delete(fileOccs);
    delete(codeOccs);

    cout << "==========================================" << endl;
    cout << "KOMPRESJA 'LZW' Z KODOWANIEM UNIWERSALNYM '" << universal_code_name << "'" << endl;
    cout << filename << " --LZW-> " << codename << endl;
    cout << "------------------------------------------" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Dlugosc pliku kodowanego: " << fileSize << endl;
    cout << "Dlugosc pliku po kompresji: " << codeSize << endl;
    cout << "Stopien kompresji: " << rate << endl;
    cout << "------------------------------------------" << endl;
    cout << "Entropia pliku kodowanego: " << fileEntropy << endl;
    cout << "Entropia pliku po kompresji: " << codeEntropy << endl;
    cout << "Srednia dlugosc kodu: " << avgLength << endl;
    cout << "==========================================" << endl;
}

void LZW_decode (string codename, string filename) {
    auto start = chrono::steady_clock::now();
    decode(codename, filename);
    auto end = chrono::steady_clock::now();

    cout << "==========================================" << endl;
    cout << "DEKOMPRESJA 'LZW'" << endl;
    cout << codename << " --(LZW)'-> " << filename << endl;
    cout << "------------------------------------------" << endl;
    cout << "Czas dekompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas dekompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "==========================================" << endl;
}
