#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

#include "bitrw.hh"
using namespace std;
typedef unsigned char uchar;
// typedef uint8_t uchar;

// TODO: zastanowic sie czy by nie wrzucic tej funkcji do BitReadera
uchar* fileToArray (string filename, size_t &n) {
    ifstream fin;
    fin.open(filename, ios::in|ios::binary);
    streampos start = fin.tellg();
    fin.ignore(numeric_limits<streamsize>::max());
    n = fin.gcount();
    fin.seekg(start,ios_base::beg);
    uchar* array = new uchar[n];
    fin.read((char *)array, n);
    fin.close();
    return array;
}

// HANDLER
BitHandler::BitHandler() {
    byteBuffer = 0;
    byteBufferIndex = -1;
}

// READER
BitReader::BitReader (string filename) : BitHandler() {
    file = fileToArray(filename, fileSize);
    fileIndex = 0;
}

BitReader::~BitReader (void) {
    delete file;
}

bool BitReader::isNextBitOne () {
    if (byteBufferIndex < 0) {
        byteBuffer = file[fileIndex];
        fileIndex++;
        byteBufferIndex = 7;
    }
    return (byteBuffer >> byteBufferIndex) & 1 > 0;
}

// WRITER
BitWriter::BitWriter (string filename) {
    clearBuffer();
    file.open(filename);
}

BitWriter::~BitWriter (void) {
    file.close();
}

void BitWriter::clearBuffer () {
    byteBuffer = 0;
    byteBufferIndex = 7;
}

void BitWriter::writeBit (int bit) {
    if (byteBufferIndex < 0) {
        file << byteBuffer;
        clearBuffer();
    }
    byteBuffer = byteBuffer*10 + bit;
    byteBufferIndex--;
}

void BitWriter::padWithZeros () {
    while (byteBufferIndex >= 0) {
        byteBuffer *= 10;
        byteBufferIndex--;
    }
    file << byteBuffer;
}

void BitWriter::closeFile () {
    file.close();
}
