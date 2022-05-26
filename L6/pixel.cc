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

void Pixel::set(Pixel* pixel) {
    blue = pixel->B();
    green = pixel->G();
    red = pixel->R();
}


PixelArray::PixelArray(size_t size) {
    length = size;
    pixels = new Pixel*[length];
    for (size_t i = 0; i < length; i++)
        pixels[i] = new Pixel();
}

PixelArray::~PixelArray() {
    for (size_t i = 0; i < length; i++)
        delete pixels[i];
    delete[] pixels;
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

PixelBitmap::PixelBitmap(const uchar _file[], size_t _width, size_t _height) {
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

uchar* pixelbitmapToArray(PixelBitmap* bitmap) {
    size_t width = bitmap->getWidth(), height = bitmap->getHeight();
    size_t arraySize = width * height * 3;
    uchar* array = new uchar[arraySize];
    size_t arrayIndex = 0;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            array[arrayIndex++] = bitmap->pixel(i, j)->B();
            array[arrayIndex++] = bitmap->pixel(i, j)->G();
            array[arrayIndex++] = bitmap->pixel(i, j)->R();
        }
    }
    return array;
}
