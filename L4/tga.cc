#include "tga.hh"

TGA::TGA(uchar* file) {
    Header = new TGAHeader(file);
    size_t idLength = Header->getIdLength();
    size_t colorMapLength = Header->getColorMapLength();
    size_t imageWidth = Header->getImageWidth();
    size_t imageHeight = Header->getImageHeight();
    size_t imageSize = imageWidth * imageHeight;
    size_t offset = 18;
    imageID = new uint8_t[idLength];
    colorMapData = new uint8_t[colorMapLength];
    imageData = new uint8_t[imageSize];
    for (size_t i = 0; i < idLength; i++)
        imageID[i] = file[offset++];
    for (size_t i = 0; i < colorMapLength; i++)
        colorMapData[i] = file[offset++];
    for (size_t i = 0; i < imageSize; i++)
        imageData[i] = file[offset++];
}

TGA::~TGA() {
    delete Header;
    delete[] imageID;
    delete[] colorMapData;
    delete[] imageData;
}

TGAHeader::TGAHeader(uchar* file) {
    idLength = file[0];
    colorMapType = file[1];
    imageType = file[2];
    firstEntryIndex = file[3];
    firstEntryIndex = (firstEntryIndex << 8) + file[4];
    colorMapLength = file[5];
    colorMapLength = (colorMapLength << 8) + file[6];
    colorMapEntrySize = file[7];
    XOrigin = file[8];
    XOrigin = (XOrigin << 8) + file[9];
    YOrigin = file[10];
    YOrigin = (YOrigin << 8) + file[11];
    imageWidth = file[12];
    imageWidth = (imageWidth << 8) + file[13];
    imageHeight = file[14];
    imageHeight = (imageHeight << 8) + file[15];
    pixelDepth = file[16];
    imageDescriptor = file[17];
}

TGAHeader::~TGAHeader() {
    //
}
