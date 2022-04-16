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
    byteBufferIndex = 0;
}

// READER
BitReader::BitReader (string filename) : BitHandler() {
    file = fileToArray(filename, fileSize);
    fileIndex = 0;
}

BitReader::~BitReader (void) {
    delete file;
}

uchar BitReader::getNextByte () {
    fileIndex++;
    if (fileIndex > fileSize) return 0;
    return file[fileIndex - 1];
}

bool BitReader::isNextBitOne () {
    if (byteBufferIndex == 0) {
        // cout << 8*fileIndex + 8 - byteBufferIndex << " " ; 
        byteBuffer = file[fileIndex];
        fileIndex++;
        byteBufferIndex = 8;
    }
    byteBufferIndex--;
    return (byteBuffer >> byteBufferIndex) & 1 > 0;
}

uint BitReader::getNextBit () {
    return isNextBitOne() ? 1 : 0;
}

void BitReader::printArray (void) {
      for (size_t i = 0; i < fileSize; i++) {
          cout << file[i] << " ";
      }
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
    byteBufferIndex = 8;
}

void BitWriter::writeBit (int bit) {
    if (byteBufferIndex == 0) {
        file << byteBuffer;
        clearBuffer();
    }
    byteBuffer = (byteBuffer << 1) + bit;
    byteBufferIndex--;
}

void BitWriter::writeByte (uchar byte) {
    if (byteBufferIndex == 8) file << byte;
    else for (size_t i = 0; i < 8; i++)
        writeBit((byte >> (7 - i)) & 1);
}

void BitWriter::writeString (string text) {
    size_t size = text.length();
    if (byteBufferIndex == 8) {
        for (size_t i = 0; i < size; i++) {
            file << text.at(i);
        }
    }
    else for (size_t j = 0; j < size; j++)
        for (size_t i = 0; i < 8; i++)
            writeBit((text.at(j) >> (7 - i)) & 1);
}

void BitWriter::padWithZeros () {
    if (byteBufferIndex != 8) {
        while (byteBufferIndex > 0) {
            byteBuffer << 1;
            byteBufferIndex--;
        }
        file << byteBuffer;
    }
}

void BitWriter::padWithZerosByte () {
    if (byteBufferIndex < 8)
        padWithZeros();
    file << 0;
    // file << endl;
}

void BitWriter::closeFile () {
    file.close();
}
