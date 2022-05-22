#include "tga.hh"

uchar* invertBitmap(const uchar bitmap[], size_t n, size_t width, size_t height) {
    uchar* inverted = new uchar[n];
    size_t invIndex = 0;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            inverted[invIndex++] = bitmap[n-3];
            inverted[invIndex++] = bitmap[n-2];
            inverted[invIndex++] = bitmap[n-1];
            n -= 3;
        }
    }
    return inverted;
}

void buildImageTGA(const string filename, const uchar bitmap[], size_t bitmapSize, size_t width, size_t height) {
    uint8_t zero1 = 0, two1 = 2, twenty_four1 = 24, dot1 = 46;
    uint16_t width2 = (uint16_t)width, height2 = (uint16_t)height;
    uint32_t zero4 = (uint32_t)0;
    ofstream f(filename);
    f << zero1;
    f << zero1;
    f << two1;
    f << zero1; f << zero1;
    f << zero1; f << zero1;
    f << zero1;
    f << zero1; f << zero1;
    f << zero1; f << zero1;
    f << width2;
    f << height2;
    f << twenty_four1;
    f << zero1;

    for (size_t i = 0; i < bitmapSize; i++)
        f << bitmap[i];
    
    f << zero4;
    f << zero4;
    f << 'T'; f << 'R'; f << 'U'; f << 'E'; f << 'V'; f << 'I'; f << 'S'; f << 'I';
    f << 'O'; f << 'N'; f << '-'; f << 'X'; f << 'F'; f << 'I'; f << 'L'; f << 'E';
    f << dot1;
    f << zero1;
    f.close();
}


TGA::TGA(const uchar* file, size_t n) {
    Header = new TGAHeader(file);
    size_t idLength = Header->getIdLength();
    size_t colorMapLength = Header->getColorMapLength();
    size_t imageWidth = Header->getImageWidth();
    size_t imageHeight = Header->getImageHeight();
    size_t imageSize = imageWidth * imageHeight;
    size_t offset = 18;
    imageID = new uchar[idLength];
    colorMapData = new uchar[colorMapLength];
    imageData = new uchar[imageSize];
    for (size_t i = 0; i < idLength; i++)
        imageID[i] = file[offset++];
    for (size_t i = 0; i < colorMapLength; i++)
        colorMapData[i] = file[offset++];
    for (size_t i = 0; i < imageSize; i++)
        imageData[i] = file[offset++];
    Footer = new TGAFooter(file, n);
}

TGA::~TGA() {
    delete Header;
    delete[] imageID;
    delete[] colorMapData;
    delete[] imageData;
    delete Footer;
}

void TGA::printTGA() {
    Header->printTGAHeader();
    cout << "---IMAGE ID---" << endl;
    for (size_t i = 0; i < Header->getIdLength(); i++)
        cout << imageID[i] << " ";
    cout << endl;
    cout << "---COLOR MAP DATA ---" << endl;
    for (size_t i = 0; i < Header->getColorMapLength(); i++)
        cout << colorMapData[i] << " ";
    cout << endl;
    cout << "---IMAGE DATA---" << endl;
    for (size_t i = 0; i < Header->getImageHeight() * Header->getImageWidth(); i++)
        cout << imageData[i] << " ";
    cout << endl;
}

SimpleTGA::SimpleTGA(const uchar file[], size_t n) {
    for (size_t i = 0; i < 18; i++)
        header[i] = file[i];

    idLength = file[0];
    colorMapType = file[1];
    imageType = file[2];
    firstEntryIndex = (file[4] << 8) + file[3];
    colorMapLength = (file[6] << 8) + file[5];
    colorMapEntrySize = file[7];
    XOrigin = (file[9] << 8) + file[8];
    YOrigin = (file[11] << 8) + file[10];
    imageWidth = (file[13] << 8) + file[12];
    imageHeight = (file[15] << 8) + file[14];
    pixelDepth = file[16];
    imageDescriptor = file[17];

    size_t offset = 18;
    bitmapSize = n - 18 - 26;
    uchar tmpBitmap[bitmapSize];
    for (size_t i = 0; i < bitmapSize; i++)
        tmpBitmap[i] = file[offset++];
    bitmap = invertBitmap(tmpBitmap, bitmapSize, imageWidth, imageHeight);
    delete[] tmpBitmap;
    pixels = new PixelBitmap(bitmap, imageWidth, imageHeight);

    for (size_t i = 0; i < 26; i++)
        footer[i] = file[n - 26 + i];
    
    offset = n - 26;
    extensionOffset = 0;
    for (size_t i = 0; i < 4; i++)
        extensionOffset = (extensionOffset << 8) + file[offset++];
    developerAreaOffset = 0;
    for (size_t i = 0; i < 4; i++)
        developerAreaOffset = (developerAreaOffset << 8) + file[offset++];
    for (size_t j = 0; j < 16; j++)
        signature[j] = file[offset++];
    end_dot = file[offset++];
    end_nul = file[offset++];
}

SimpleTGA::~SimpleTGA() {
    delete[] bitmap;
    delete[] header;
    delete[] footer;
    delete pixels;
}

void SimpleTGA::printSimpleTGA() {
    // uint8_t zero = 0;
    // cout << zero << endl;
    // cout << (int)zero << endl;
    cout << "---HEADER---" << endl;
    cout << "ID Length: " << (int)idLength << endl;
    cout << "Color Map Type: " << (int)colorMapType << endl;
    cout << "Image Type: " << (int)imageType << endl;
    cout << "First Entry Index: " << (int)firstEntryIndex << endl;
    cout << "Color Map Length: " << (int)colorMapLength << endl;
    cout << "Color Map Entry Size: " << (int)colorMapEntrySize << endl;
    cout << "X Origin: " << (int)XOrigin << endl;
    cout << "Y Origin: " << (int)YOrigin << endl;
    cout << "Image Width: " << (int)imageWidth << endl;
    cout << "Image Height: " << (int)imageHeight << endl;
    cout << "Pixel Depth: " << (int)pixelDepth << endl;
    cout << "Image Descriptor: " << (int)imageDescriptor << endl;

    // cout << "---BITMAP---" << endl;
    // for (size_t i = 0; i < bitmapSize; i++)
    //     cout << bitmap[i] << " ";
    // cout << endl;

    cout << "---FOOTER---" << endl;
    cout << "Extension Offset: " << (int)extensionOffset << endl;
    cout << "Developer Area Offset: " << (int)developerAreaOffset << endl;
    cout << "Signature:";
    for (size_t i = 0; i < 16; i++)
        cout << " " << signature[i];
    cout << endl;
    cout << "End Dot: " << (int)end_dot << endl;
    cout << "End Null: " << (int)end_nul << endl;
}

uchar* SimpleTGA::arrayToTGA(uchar* array, size_t size) {
    size_t n = 18 + size + 26;
    uchar* tga = new uchar[n];
    for (size_t i = 0; i < 18; i++)
        tga[i] = footer[i];
    
    for (size_t i = 0; i < size; i++)
        tga[18 + i] = array[i];
    
    for (size_t i = 0; i < 26; i++)
        tga[n - 26 + i] = footer[i];
}

TGAHeader::TGAHeader(const uchar* file) {
    idLength = file[0];
    colorMapType = file[1];
    imageType = file[2];
    firstEntryIndex = (file[4] << 8) + file[3];
    colorMapLength = (file[6] << 8) + file[5];
    colorMapEntrySize = file[7];
    XOrigin = (file[9] << 8) + file[8];
    YOrigin = (file[11] << 8) + file[10];
    imageWidth = (file[13] << 8) + file[12];
    imageHeight = (file[15] << 8) + file[14];
    pixelDepth = file[16];
    imageDescriptor = file[17];
}

TGAHeader::~TGAHeader() {
    //
}

void TGAHeader::printTGAHeader() {
    cout << "---HEADER---" << endl;
    cout << "ID Length: " << idLength << endl;
    cout << "Color Map Type: " << colorMapType << endl;
    cout << "Image Type: " << imageType << endl;
    cout << "First Entry Index: " << firstEntryIndex << endl;
    cout << "Color Map Length: " << colorMapLength << endl;
    cout << "Color Map Entry Size: " << colorMapEntrySize << endl;
    cout << "X Origin: " << XOrigin << endl;
    cout << "Y Origin: " << YOrigin << endl;
    cout << "Image Width: " << imageWidth << endl;
    cout << "Image Height: " << imageHeight << endl;
    cout << "Pixel Depth: " << pixelDepth << endl;
    cout << "Image Descriptor: " << imageDescriptor << endl;
}

TGAFooter::TGAFooter(const uchar* file, size_t n) {
    size_t offset = n - 26;
    extensionOffset = 0;
    for (size_t i = 0; i < 4; i++)
        extensionOffset = (extensionOffset << 8) + file[offset++];
    developerAreaOffset = 0;
    for (size_t i = 0; i < 4; i++)
        developerAreaOffset = (developerAreaOffset << 8) + file[offset++];
    for (size_t j = 0; j < 16; j++)
        signature[j] = file[offset++];
    end_dot = file[offset++];
    end_nul = file[offset++];
}

TGAFooter::~TGAFooter() {
    //
}

void TGAFooter::printTGAFooter() {
    cout << "---FOOTER---" << endl;
    cout << "Extension Offset: " << extensionOffset << endl;
    cout << "Developer Area Offset: " << developerAreaOffset << endl;
    cout << "Signature:";
    for (size_t i = 0; i < 16; i++)
        cout << " " << signature[i];
    cout << endl;
    cout << "End Dot: " << end_dot << endl;
    cout << "End Null: " << end_nul << endl;
}
