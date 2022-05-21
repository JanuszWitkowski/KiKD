#ifndef __TGA_H
#define __TGA_H

#include "def.hh"
#include "bitrw.hh"
#include "pixel.hh"

using namespace std;

enum ColorBGRType {
    BLUE,
    GREEN,
    RED,
    ALL
};

Pixel** bitmapToBGR(uchar* array, size_t n, size_t width, size_t height);

class TGAHeader {
private:
    uchar idLength;

    uchar colorMapType;

    uchar imageType;

    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uchar colorMapEntrySize;

    uint16_t XOrigin;
    uint16_t YOrigin;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uchar pixelDepth;
    uchar imageDescriptor;

public:
    TGAHeader(uchar* file);
    virtual ~TGAHeader();
    void printTGAHeader();
    uchar getIdLength() { return idLength; }
    uchar getColorMapLength() { return colorMapLength; }
    uchar getImageWidth() { return imageWidth; }
    uchar getImageHeight() { return imageHeight; }
};

class TGAFooter {
private:
    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
    __uint128_t signature;
    uchar end_dot;
    uchar end_nul;
public:
    TGAFooter(uchar* file, size_t n);
    virtual ~TGAFooter();
    void printTGAFooter();
};

class TGA {
private:
    TGAHeader* Header;

    uchar* imageID;
    uchar* colorMapData;
    uchar* imageData;

    TGAFooter* Footer;
public:
    TGA(uchar* file, size_t n);
    virtual ~TGA();
    void printTGA();
    TGAHeader* getHeader() { return Header; }
    uchar* getImageID() { return imageID; }
    uchar* getColorMapData() { return colorMapData; }
    uchar* getImageData() { return imageData; }
};

class SimpleTGA {
private:
    size_t bitMapSize;
    uchar* bitMap;
public:
    SimpleTGA(uchar* file, size_t n);
    virtual ~SimpleTGA();
    void printSimpleTGA();

    uchar idLength;
    uchar colorMapType;
    uchar imageType;
    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uchar colorMapEntrySize;
    uint16_t XOrigin;
    uint16_t YOrigin;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uchar pixelDepth;
    uchar imageDescriptor;

    uchar* getBitMap() { return bitMap; }
    size_t getBitMapSize() { return bitMapSize; }

    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
    __uint128_t signature;
    uchar end_dot;
    uchar end_nul;
};

#endif