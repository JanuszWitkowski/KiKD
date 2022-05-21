#include "pixel.hh"

Pixel::Pixel() {
    blue = 0;
    green = 0;
    red = 0;
}

Pixel::Pixel(uchar Blue, uchar Green, uchar Red) {
    blue = Blue;
    green = Green;
    red = Red;
}

Pixel::~Pixel() {
    //
}


PixelBitmap::PixelBitmap(size_t _width, size_t _height) {
    width = _width;
    height = _height;
    bitmap = new Pixel**[height];
    for (size_t i = 0; i < height; i++) {
        bitmap[i] = new Pixel*[width];
        for (size_t j = 0; j < width; j++)
            bitmap[i][j] = new Pixel();
    }
}

PixelBitmap::PixelBitmap(uchar _file[], size_t _width, size_t _height) {
    width = _width;
    height = _height;
    size_t fileIndex = 0;
    bitmap = new Pixel**[height];
    for (size_t i = 0; i < height; i++) {
        bitmap[i] = new Pixel*[width];
        for (size_t j = 0; j < width; j++)
            bitmap[i][j] = new Pixel(_file[fileIndex++], _file[fileIndex++], _file[fileIndex++]);
    }
}

PixelBitmap::~PixelBitmap() {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            delete bitmap[i][j];
        }
        delete[] bitmap[i];
    }
    delete[] bitmap;
}
