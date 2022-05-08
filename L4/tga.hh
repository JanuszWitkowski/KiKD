#ifndef __TGA_H
#define __TGA_H

#include <iostream>
#include "bitrw.hh"

using namespace std;

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
    uint8_t getIdLength() { return idLength; }
    uint8_t getColorMapLength() { return colorMapLength; }
    uint8_t getImageWidth() { return imageWidth; }
    uint8_t getImageHeight() { return imageHeight; }
};

class TGAFooter {
private:
    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
public:
    TGAFooter();
    virtual ~TGAFooter();
};

class TGA {
private:
    TGAHeader* Header;

    uint8_t* imageID;
    uint8_t* colorMapData;
    uint8_t* imageData;
public:
    TGA(uchar* file);
    virtual ~TGA();
};

#endif