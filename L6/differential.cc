#include "differential.hh"
#include <fstream>

double averageValue (uchar xn, uchar xm) {
    // cout << (int)xn << "->" << (double)xn << " " << (int)xm << "->" << (double)xm << " =+= " << ((double)xn + (double)xm)/2.0 << endl;
    return ((double)xn + (double)xm)/2.0; 
}

double deviationValue (uchar xn, uchar xm) {
    // cout << (int)xn << "->" << (double)xn << " " << (int)xm << "->" << (double)xm << " =-= " << ((double)xn - (double)xm)/2.0 << endl;
    return ((double)xn - (double)xm)/2.0;
}


double* filterAverage (uchar* x, size_t n) {
    double* filter = new double[n];
    // filter[0] = ((double)(x[0]))/2.0;
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = averageValue(x[i], x[i-1]);
    }
    return filter;
}

double* filterDeviation (uchar* x, size_t n) {
    double* filter = new double[n];
    // filter[0] = ((double)(x[0]))/2.0;
    filter[0] = x[0];
    for (size_t i = 1; i < n; i++) {
        filter[i] = deviationValue(x[i], x[i-1]);
    }
    return filter;
}


uchar quantize (double x, int* q, size_t qSize) {
    size_t left = 1, right = qSize - 1;
    while (left <= right) {
        size_t center = (left + right)/2;
        if (x < (double)(q[center])) {
            if (x >= (double)(q[center-1])) {
                return center - 1;
            }
            else {
                right = center - 1;
            }
        }
        else if (x > (double)(q[center])) {
            if (x <= (double)(q[center+1])) {
                return center;
            }
            else {
                left = center + 1;
            }
        }
        else {
            if (center == qSize-1) {
                return center - 1;
            }
            else {
                return center;
            }
        }
    }
    cerr << cRed << "ERROR: BŁĄD PRZY PRÓBIE KWANTYZACJI WARTOŚCI " << x << cReset << endl;
    return 0;
}

uchar* differentialCoding (double* a, size_t aSize, size_t qBits) {
    // ofstream f("output/debug0.txt");
    // for (size_t i = 0; i < aSize; i++)
    //     f << a[i] << endl;
    // f.close();
    size_t qSize = (1 << qBits) + 1;
    int qinit[qSize];
    uchar qinitNext = (1 << (8 - qBits));
    qinit[0] = 0;
    for (size_t i = 1; i < qSize; i++) {
        qinit[i] = qinit[i-1] + qinitNext;
    }

    int q[qSize];
    uchar qNext = (1 << (9 - qBits));
    uchar qHalf = (qNext >> 1);
    q[0] = -256;
    for (size_t i = 1; i < qSize; i++) {
        q[i] = q[i-1] + qNext;
    }
    
    double diffs[aSize];
    uchar* d = new uchar[aSize];
    uchar aNew[aSize];
    diffs[0] = a[0];
    d[0] = quantize(diffs[0], qinit, qSize);
    // aNew[0] = d[0];
    aNew[0] = floor(a[0]);
    // ofstream fout("output/debug1.txt");
    // fout << (int)(aNew[0]) << endl;

    for (size_t i = 1; i < aSize; i++) {
        diffs[i] = a[i] - (double)(aNew[i-1]);
        d[i] = quantize(diffs[i], q, qSize);
        aNew[i] = q[d[i]] + qHalf + aNew[i-1];
        // fout << (int)(aNew[i]) << endl;
    }

    // fout.close();
    return d;
}

uchar* straightQuantizing (double* a, size_t aSize, size_t qBits) {
    size_t qSize = (1 << qBits) + 1;
    int q[qSize];
    uchar qNext = (1 << (8 - qBits));
    uchar qHalf = (qNext >> 1);
    double delta = 255.0/2.0;
    q[0] = 0;
    for (size_t i = 1; i < qSize; i++) {
        q[i] = q[i-1] + qNext;
    }
    uchar* output = new uchar[aSize];
    output[0] = quantize(a[0], q, qSize);
    for (size_t i = 1; i < aSize; i++) {
        output[i] = quantize(a[i] + delta, q, qSize);
    }
    return output;
}


void printBandsToFile (string filename, uchar** downs, uchar** ups, size_t width, size_t height, size_t qBits) {
    BitWriter writer(filename);
    cout << "!!!" << width << " " << height << endl;
    writer.writeByte((uchar)((width >> 8) & 255));
    writer.writeByte((uchar)(width & 255));
    writer.writeByte((uchar)((height >> 8) & 255));
    writer.writeByte((uchar)(height & 255));
    writer.writeByte((uchar)qBits);
    for (size_t i = 0; i < width*height; i++) {
        for (size_t color = 0; color < 3; color++) {
            for (int bits = qBits-1; bits >= 0; bits--) {
                writer.writeBit((((downs[color][i]) >> bits) & 1));
            }
        }
    }
    for (size_t i = 0; i < width*height; i++) {
        for (size_t color = 0; color < 3; color++) {
            for (int bits = qBits-1; bits >= 0; bits--) {
                writer.writeBit((((ups[color][i]) >> bits) & 1));
            }
        }
    }
    writer.padWithZerosByte();
}



FilterHolder::FilterHolder(size_t size) {
    length = size;
    filter = new double*[colorsNumber];
    for (size_t color = 0; color < colorsNumber; color++) {
        filter[color] = new double[length];
        for (size_t i = 0; i < length; i++) {
            filter[color][i] = 0.0;
        }
    }
}

FilterHolder::~FilterHolder() {
    for (size_t color = 0; color < colorsNumber; color++)
        delete[] filter[color];
    delete[] filter;
}

BandSolver::BandSolver(size_t size) {
    length = size;
    filters = new int**[bandsNumber];
    codings = new uchar**[bandsNumber];
    for (size_t band = 0; band < bandsNumber; band++) {
        filters[band] = new int*[colorsNumber];
        codings[band] = new uchar*[colorsNumber];
        for (size_t color = 0; color < colorsNumber; color++) {
            filters[band][color] = new int[length];
            codings[band][color] = new uchar[length];
            for (size_t i = 0; i < length; i++) {
                filters[band][color][i] = 0;
                codings[band][color][i] = 0;
            }
        }
    }
    bitmap = new uchar[length*3];
    for (size_t i = 0; i < length*3; i++) {
        bitmap[i] = 0;
    }
}

BandSolver::BandSolver(string filename) {
    // READING FILE
    BitReader reader(filename);
    width = reader.getNextByte();
    width = (width << 8) + reader.getNextByte();
    height = reader.getNextByte();
    height = (height << 8) + reader.getNextByte();
    qBits = reader.getNextByte();
    length = width*height;
    cout << "!!" << qBits << " " << height << " " << width << endl;
    codings = new uchar**[bandsNumber];
    for (size_t band = 0; band < bandsNumber; band++) {
        codings[band] = new uchar*[colorsNumber];
        for (size_t j = 0; j < colorsNumber; j++) {
            codings[band][j] = new uchar[length];
        }
        for (size_t i = 0; i < length; i++) {
            for (size_t color = 0; color < colorsNumber; color++) {
                uchar v = 0;
                for (size_t bits = 0; bits < qBits; bits++) {
                    v = (v << 1) + reader.getNextBit();
                }
                codings[band][color][i] = v;
            }
        }
    }

    // GENERATING QUNATIZERS
    size_t qSize = (1 << qBits) + 1;
    int q1[qSize];
    uchar q1Next = (1 << (8 - qBits));
    uchar q1Half = (q1Next >> 1);
    q1[0] = 0;
    for (size_t i = 1; i < qSize; i++) {
        q1[i] = q1[i-1] + q1Next;
    }
    int q0[qSize];
    uchar q0Next = (1 << (9 - qBits));
    uchar q0Half = (q0Next >> 1);
    q0[0] = -256;
    for (size_t i = 1; i < qSize; i++) {
        q0[i] = q0[i-1] + q0Next;
    }
    int delta = 255/2;

    // RECONSTRUCTING FILTERS
    filters = new int**[bandsNumber];
    for (size_t i = 0; i < bandsNumber; i++) {
        filters[i] = new int*[colorsNumber];
        for (size_t j = 0; j < colorsNumber; j++) {
            filters[i][j] = new int[length];
        }
    }
    // ofstream fout("output/debug2.txt");
    for (size_t color = 0; color < colorsNumber; color++) {
        filters[0][color][0] = q1[codings[0][color][0]] + q1Half;
        filters[1][color][0] = q1[codings[1][color][0]] + q1Half;
        // if (color == RED) fout << filters[0][color][0] << endl;
        for (size_t i = 1; i < length; i++)  {
            filters[0][color][i] = filters[0][color][i-1] + q0[codings[0][color][i]] + q0Half;
            // filters[0][color][i] = filters[0][color][i-1] + q0[codings[0][color][i]];
            // filters[0][color][i] = q0[codings[0][color][i]];

            filters[1][color][i] = q1[codings[1][color][i]] + q1Half - delta;
            // filters[1][color][i] = q1[codings[1][color][i]];
            // if (color == RED) fout << filters[0][color][i] << endl;
        }
    }
    // fout.close();

    // SOLVING BITMAP
    bitmap = new uchar[length*3];
    for (size_t color = 0; color < colorsNumber; color++) {
        bitmap[0 + color] = filters[0][color][0];
    }
    for (size_t i = 1; i < length; i++) {
        for (size_t color = 0; color < colorsNumber; color++) {
            bitmap[3*i + color] = \
                        floor(filters[0][color][i] + filters[1][color][i]);
                        // floor(filters[0][color][i]);
                        // floor(filters[1][color][i]);
        }
    }
}

BandSolver::~BandSolver() {
    for (size_t band = 0; band <bandsNumber; band++) {
        for (size_t color = 0; color < colorsNumber; color++) {
            delete[] filters[band][color];
            delete[] codings[band][color];
        }
        delete[] filters[band];
        delete[] codings[band];
    }
    delete[] filters;
    delete[] codings;
    delete[] bitmap;
}



double mse(PixelBitmap* bitmap1, PixelBitmap* bitmap2) {
    double sum = 0.0;
    for (size_t i = 0; i < bitmap1->getHeight(); i++) {
        for (size_t j = 0; j < bitmap1->getWidth(); j++) {
            double* pixelOld = getPixelAsDoubleArray(bitmap1->pixel(i, j));
            double* pixelNew = getPixelAsDoubleArray(bitmap2->pixel(i, j));
            sum += euclidSquared(pixelOld, pixelNew);
            delete[] pixelOld;
            delete[] pixelNew;
        }
    }
    return sum / (bitmap1->getWidth() * bitmap1->getHeight());
}

double snr(SimpleTGA* tga, double error) {
    double sum = 0.0;
    for (size_t i = 0; i < tga->imageHeight; i++) {
        for (size_t j = 0; j < tga->imageWidth; j++) {
            sum += pow(tga->getPixelBitmap()->pixel(i, j)->B(), 2.0) +
                pow(tga->getPixelBitmap()->pixel(i, j)->G(), 2.0) +
                pow(tga->getPixelBitmap()->pixel(i, j)->R(), 2.0);
        }
    }
    return sum / (error * (double)(tga->imageWidth) * (double)(tga->imageHeight));
}

double euclidSquared(double a[], double b[]) {
    double sum = 0.0;
    for (size_t i = 0; i < 3; i++) {
        sum += pow((double)(a[i] - b[i]), 2.0);
    }
    return sum;
}

double* getPixelAsDoubleArray(Pixel* a) {
    double* x = new double[3];
    x[0] = (double)(a->B());
    x[1] = (double)(a->G());
    x[2] = (double)(a->R());
    return x;
}
