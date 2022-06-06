#ifndef __TGA_H
#define __TGA_H

#include <fstream>
#include "consts.hh"
#include "bitrw.hh"
#include "pixel.hh"

using namespace std;

enum ColorBGRType {
    BLUE,
    GREEN,
    RED,
    ALL
};

uchar* invertBitmap(const uchar bitmap[], size_t n, size_t width, size_t height);
void buildImageTGA(const string filename, const uchar bitmap[], size_t bitmapSize, size_t width, size_t height);
void printArrayToFile(const string filename, const uchar* array, size_t width, size_t height);

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
    TGAHeader(const uchar* file);
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
    uint8_t signature[16];
    uint8_t end_dot;
    uint8_t end_nul;
public:
    TGAFooter(const uchar* file, size_t n);
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
    TGA(const uchar* file, size_t n);
    virtual ~TGA();
    void printTGA();
    TGAHeader* getHeader() { return Header; }
    uchar* getImageID() { return imageID; }
    uchar* getColorMapData() { return colorMapData; }
    uchar* getImageData() { return imageData; }
};

class SimpleTGA {
private:
    size_t bitmapSize;
    uchar* bitmap;
    PixelBitmap* pixels;
    uchar header[18];
    uchar footer[26];
public:
    SimpleTGA(const uchar file[], size_t n);
    SimpleTGA(const string filename, const uchar file[], size_t n);
    virtual ~SimpleTGA();
    void printSimpleTGA();
    uchar* arrayToTGA(uchar* array, size_t size);
    void produceTGAFile(string filename, uchar* file, size_t n);

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

    uint8_t* getBitmap() { return bitmap; }
    size_t getBitmapSize() { return bitmapSize; }
    PixelBitmap* getPixelBitmap() { return pixels; }

    uint32_t extensionOffset;
    uint32_t developerAreaOffset;
    uint8_t signature[16];
    uint8_t end_dot;
    uint8_t end_nul;
};

#endif