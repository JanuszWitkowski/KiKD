#ifndef __TGA_H
#define __TGA_H

#include <iostream>
#include "bitrw.hh"

using namespace std;

enum ColorBGRType {
    BLUE,
    GREEN,
    RED,
    ALL
};

uint8_t*** bitmapToBGR(uint8_t* array, size_t n, size_t width, size_t height);
uint8_t* codesToBitmap(uint8_t** array, size_t colorSize);

class TGAHeader {
private:
    uint8_t idLength;

    uint8_t colorMapType;

    uint8_t imageType;

    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uint8_t colorMapEntrySize;

    uint16_t XOrigin;
    uint16_t YOrigin;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint8_t pixelDepth;
    uint8_t imageDescriptor;

public:
    TGAHeader(uchar* file);
    virtual ~TGAHeader();
    void printTGAHeader();
    uint8_t getIdLength() { return idLength; }
    uint8_t getColorMapLength() { return colorMapLength; }
    uint8_t getImageWidth() { return imageWidth; }
    uint8_t getImageHeight() { return imageHeight; }
};

class TGAFooter {
private:
    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
    __uint128_t signature;
    uint8_t end_dot;
    uint8_t end_nul;
public:
    TGAFooter(uchar* file, size_t n);
    virtual ~TGAFooter();
    void printTGAFooter();
};

class TGA {
private:
    TGAHeader* Header;

    uint8_t* imageID;
    uint8_t* colorMapData;
    uint8_t* imageData;

    TGAFooter* Footer;
public:
    TGA(uchar* file, size_t n);
    virtual ~TGA();
    void printTGA();
    TGAHeader* getHeader() { return Header; }
    uint8_t* getImageID() { return imageID; }
    uint8_t* getColorMapData() { return colorMapData; }
    uint8_t* getImageData() { return imageData; }
};

class SimpleTGA {
private:
    size_t bitMapSize;
    uint8_t* bitMap;
public:
    SimpleTGA(uchar* file, size_t n);
    virtual ~SimpleTGA();
    void printSimpleTGA();

    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;
    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uint8_t colorMapEntrySize;
    uint16_t XOrigin;
    uint16_t YOrigin;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint8_t pixelDepth;
    uint8_t imageDescriptor;

    uint8_t* getBitMap() { return bitMap; }
    size_t getBitMapSize() { return bitMapSize; }

    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
    __uint128_t signature;
    uint8_t end_dot;
    uint8_t end_nul;
};

#endif