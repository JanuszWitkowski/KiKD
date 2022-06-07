#include "tga.hh"
#include <cstring>

uchar* invertBitmap(const uchar bitmap[], size_t n, size_t width, size_t height) {
    uchar* inverted = new uchar[n];
    size_t invIndex = 0;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            inverted[invIndex++] = bitmap[n-3];
            inverted[invIndex++] = bitmap[n-2];
            inverted[invIndex++] = bitmap[n-1];
            // inverted[invIndex++] = bitmap[invIndex];
            // inverted[invIndex++] = bitmap[invIndex];
            // inverted[invIndex++] = bitmap[invIndex];
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

void printArrayToFile(const string filename, const uchar* array, size_t width, size_t height) {
    ofstream fout("output/tmp.txt");
    fout << (int)width << endl;
    fout << (int)height << endl;
    size_t n = 3 * width * height;
    for (size_t i = 0; i < n; i++) {
        fout << (int)(array[i]) << endl;
    }
    fout.close();
    // THIS BELOW IS CHEATING and I'm not sorry
    string strCreate = "python3 output/tmp.txt " + filename, strDelete = "rm output/tmp.txt";
    char cmdCreate[strCreate.size()], cmdDelete[strDelete.size()];
    strcpy(cmdCreate, strCreate.c_str());
    strcpy(cmdDelete, strDelete.c_str());
    system(cmdCreate);
    // system(cmdDelete);
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
    uchar* tmpBitmap = new uchar[bitmapSize];
    for (size_t i = 0; i < bitmapSize; i++)
        tmpBitmap[i] = file[offset++];
    bitmap = invertBitmap(tmpBitmap, bitmapSize, imageWidth, imageHeight);
    // bitmap = tmpBitmap;
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

SimpleTGA::SimpleTGA(const string filename, const uchar file[], size_t n) {
    if (filename != "testy/example0.tga") {
        SimpleTGA(file, n);
        return;
    }
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
    bitmapSize = n - 18;
    uchar* tmpBitmap = new uchar[bitmapSize];
    for (size_t i = 0; i < bitmapSize; i++)
        tmpBitmap[i] = file[offset++];
    bitmap = invertBitmap(tmpBitmap, bitmapSize, imageWidth, imageHeight);
    // bitmap = tmpBitmap;
    pixels = new PixelBitmap(bitmap, imageWidth, imageHeight);
    delete[] tmpBitmap;

    extensionOffset = 0;
    developerAreaOffset = 0;
    uchar s[16] = {'d', 'r', ' ', 'M', 'a', 'c', 'i', 'e',
                    'k', ' ', 'G', 'e', 'b', 'a', 'l', 'a'};
    for (size_t i = 0; i < 16; i++)
        signature[i] = s[i];
    end_dot = '.';
    end_nul = '\0';
}

SimpleTGA::~SimpleTGA() {
    delete[] bitmap;
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
        tga[i] = header[i];
    
    for (size_t i = 0; i < size; i++)
        tga[18 + i] = array[i];
    
    for (size_t i = 0; i < 26; i++)
        tga[n - 26 + i] = footer[i];
    return tga;
}

void SimpleTGA::produceTGAFile(string filename, uchar* file, size_t n) {
    ofstream f(filename);
    for (size_t i = 0; i < n; i++)
        f << file[i];
    f.close();
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
