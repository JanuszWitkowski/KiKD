#include "tga.hh"

Pixel** bitmapToBGR(const uchar* array, size_t n, size_t width, size_t height) {
    size_t arrayIndex = 0;
    Pixel** BGR = new Pixel*[height];
    size_t m = 0;
    for (size_t i = 0; i < height; i++) {
        BGR[i] = new Pixel[width];
        for (size_t j = 0; j < width; j++) {
            BGR[i][j].B(array[m++]);
            BGR[i][j].G(array[m++]);
            BGR[i][j].R(array[m++]);
        }
    }
    return BGR;
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

SimpleTGA::SimpleTGA(const uchar* file, size_t n) {
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
    bitMapSize = n - 18 - 26;
    bitMap = new uchar[bitMapSize];
    for (size_t i = 0; i < bitMapSize; i++)
        bitMap[i] = file[offset++];
    
    offset = n - 26;
    extensionOffset = 0;
    for (size_t i = 0; i < 4; i++)
        extensionOffset = (extensionOffset << 8) + file[offset++];
    developerAreaOffset = 0;
    for (size_t i = 0; i < 4; i++)
        developerAreaOffset = (developerAreaOffset << 8) + file[offset++];
    signature = 0;
    for (size_t j = 0; j < 16; j++)
        signature = (signature << 8) + file[offset++];
    end_dot = file[offset++];
    end_nul = file[offset++];
}

SimpleTGA::~SimpleTGA() {
    delete[] bitMap;
}

void SimpleTGA::printSimpleTGA() {
    // Header->printTGAHeader();
    cout << "---BITMAP---" << endl;
    for (size_t i = 0; i < bitMapSize; i++)
        cout << bitMap[i] << " ";
    cout << endl;
    // Footer->printTGAFooter();
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
    signature = 0;
    for (size_t j = 0; j < 16; j++)
        signature = (signature << 8) + file[offset++];
    end_dot = file[offset++];
    end_nul = file[offset++];
}

TGAFooter::~TGAFooter() {
    //
}

void TGAFooter::printTGAFooter() {
    cout << "---FOOTER---" << endl;
}
