#ifndef __BITRW_H
#define __BITRW_H

#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
using namespace std;

// typedef unsigned char uchar;
typedef uint8_t uchar;

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
    BitReader(const string filename);
    virtual ~BitReader (void);
    uchar getNextByte();
    bool isNextBitOne();
    uint getNextBit();
    void printArray();

    uint getCurrentBit() { return (byteBuffer >> byteBufferIndex) & 1; }
    uchar getByte(size_t index) { return file[index]; }
    uchar* getFileArray() { return file; }
    size_t getFileSize() { return fileSize; }
    size_t getFileSizeInBits() { return 8*fileSize; }
    bool isEOF() { return fileIndex >= fileSize; }
};

class BitWriter : public BitHandler {
private:
    ofstream file;
    void clearBuffer();
public:
    BitWriter(const string filename);
    virtual ~BitWriter (void);
    void writeBit(int bit);
    void writeByte(uchar byte);
    void writeString(const string text);
    void padWithZeros();
    void padWithZerosByte();
    void closeFile();
};

uchar* fileToArray (const string filename, size_t &n);
void printArray (const uchar* array, size_t n);

#endif
