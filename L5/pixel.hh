#ifndef __PIXEL_H
#define __PIXEL_H

#include "def.hh"

class Pixel {
private:
    uchar red;
    uchar green;
    uchar blue;
public:
    Pixel();
    Pixel(uchar Blue, uchar Green, uchar Red);
    virtual ~Pixel();
    uchar R() { return red; }
    uchar G() { return green; }
    uchar B() { return blue; }
    void R(const uchar v) { red = v; }
    void G(const uchar v) { green = v; }
    void B(const uchar v) { blue = v; }
    void set(uchar b, uchar g, uchar r) { R(r); G(g); B(b); }
    void set(Pixel* pixel);
};

class PixelBitmap {
private:
    size_t width;
    size_t height;
    Pixel*** bitmap;
public:
    PixelBitmap(size_t _width, size_t _height);
    PixelBitmap(const uchar _file[], size_t _width, size_t _height);
    virtual ~PixelBitmap();
    size_t getWidth() { return width; }
    size_t getHeight() { return height; }
    Pixel* pixel(size_t i, size_t j) { return bitmap[i][j]; }
};

uchar* pixelbitmapToArray(PixelBitmap* bitmap);

#endif