#ifndef __BITRW_H
#define __BITRW_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;

typedef unsigned char uchar;
// typedef uint8_t uchar;

class BitHandler {
protected:
    uchar byteBuffer;
    size_t byteBufferIndex;
public:
    BitHandler();
};

class BitReader : public BitHandler {
private:
    uchar* file;
    size_t fileSize;
    size_t fileIndex;
public:
    BitReader(string filename);
    virtual ~BitReader (void);
    bool isNextBitOne();
    void printArray();

    uchar* getFileArray () {
        return file;
    }

    size_t getFileSize () {
        return fileSize;
    }
};

class BitWriter : public BitHandler {
private:
    ofstream file;
    void clearBuffer();
public:
    BitWriter(string filename);
    virtual ~BitWriter (void);
    void writeBit(int bit);
    void padWithZeros();
    void closeFile();
};

uchar* fileToArray (string filename, size_t &n);

#endif
