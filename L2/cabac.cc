#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <chrono>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include "entropy.hh"
#include "cabac.hh"
using namespace std;
// using boost::multiprecision::cpp_dec_float_50;
using boost::multiprecision::cpp_dec_float_100;
// using boost::multiprecision::cpp_dec_float;

typedef unsigned char uchar;
// typedef uint8_t uchar;
// typedef cpp_dec_float_50 fdec;
// typedef cpp_dec_float_100 fdec;
typedef cpp_dec_float_100 bigfloat;
// typedef boost::multiprecision::number<cpp_dec_float<200> > fdec;
typedef double fdec;

const int BLOCK_SIZE = 256;

ArithmeticCode::ArithmeticCode(int length, fdec value) {
// ArithmeticCode::ArithmeticCode(int length, vector<fdec> vector) {
    setN(length);
    setTag(value);
    // tags = vector;
}

uchar* setBlock (uchar* array, uchar* block, int start, int m, int n) {
    if (start + m > n) {
        delete block;
        m = n - start;
        block = new uchar[m];
    }
    for (int i = 0; i < m; i++) {
        block[i] = array[start + i];
    }
    return block;
}

void updateCharOccs (int* occs, uchar* array, int n) {
    for (int i = 0; i < n; i++) {
        occs[array[i]]++;
    }
}

int sumOfOccs (int* occs, uchar symbol) {
    int sum = 0;
    for (int i = symbol - 1; i >= 0; i--) {
        sum += occs[i];
    }
    return sum;
}

unsigned int countBuffer (int* occs, int n) {
    int j;
    unsigned int m;
    double mult = 1.0, prev = 1.0, value = 0.0;
    double inf = std::numeric_limits<double>::infinity();
    for (int i = 0; i < 256; i++) {
        prev = mult;
        mult *= occs[i];
        if (mult >= inf) {
            value += (i - j)*log2(n) - log2(prev);
            mult = 1.0*occs[i];
        }
        cout << "occs[" << i << "] = " << occs[i] << "; mult = " << mult << endl;
    }
    value += (256 - j)*log2(n) - log2(mult);
    m = ceil(value) + 1;
    std::cout << "m = " << m << endl;
    return 0;
}

void printCharOccs (int* occs) {
    for (int i = 0; i < 256; i++) {
        cout << "occs[" << i << "] = " << occs[i] << endl;
    }
}

double stringToDouble (string s) {
    double value = 0.0;
    for (int i = 1; i <= s.length(); i++) {
        if (s.at(i-1) == '1') value += pow(2.0, -i);
    }
    return value;
}

ArithmeticCode *encode (uchar* array, int n, int b, string codename) {
    fdec tagDouble;
    string tag = "";        // prawdziwy znacznik
    string line = "";
    fdec l = 0, r = 1, d;   // bounds
    int F;      // distribution
    unsigned int m;
    uchar buffer = 0;
    int bufferCounter = 0;
    uchar* block = new uchar[b];
    uchar symbol;
    int* charOccs = initCharOccs(1);    // ustaw liczbe wystapien kazdego znaku na 1
    int allOccs = 256;
    int counter = 0;
    bool do_scaling = true;

    ifstream tmpin;
    ofstream fout;
    ofstream tmpout("temp.cps");

    for (int i = 0; i < n; i += b) {
        block = setBlock(array, block, i, b, n);
        for (int j = 0; j < b; j++) {
            symbol = block[j];
            F = sumOfOccs(charOccs, symbol);
            d = r - l;
            r = l + (((double)F + (double)charOccs[symbol]) / (double)allOccs) * d;     // dzielenie!!!
            l = l + ((double)F / (double)allOccs) * d;                          // dzielenie!!!
            do {
                do_scaling = false;
                if (r <= 0.5) {
                    do_scaling = true;
                    l = 2*l;
                    r = 2*r;
                    // tmpout << 0;
                    buffer = buffer << 1;
                    bufferCounter++;
                    if (bufferCounter == 8) {
                        tmpout << buffer;
                        buffer = 0;
                        bufferCounter = 0;
                    }
                    for (; counter > 0; counter--) {
                        // tmpout << 1;
                        buffer = (buffer << 1) + 1;
                        bufferCounter++;
                        if (bufferCounter == 8) {
                            tmpout << buffer;
                            buffer = 0;
                            bufferCounter = 0;
                        }
                    }
                    counter = 0;
                }
                if (0.5 <= l) {
                    do_scaling = true;
                    l = 2*l - 1;
                    r = 2*r - 1;
                    // tmpout << 1;
                    buffer = (buffer << 1) + 1;
                    bufferCounter++;
                    if (bufferCounter == 8) {
                        tmpout << buffer;
                        buffer = 0;
                        bufferCounter = 0;
                    }
                    for (; counter > 0; counter--) {
                        // tmpout << 0;
                        buffer = buffer << 1;
                        bufferCounter++;
                        if (bufferCounter == 8) {
                            tmpout << buffer;
                            buffer = 0;
                            bufferCounter = 0;
                        }
                    }
                    counter = 0;
                }
                if (l < 0.5 && 0.5 < r && 0.25 <= l && r <= 0.75) {
                    do_scaling = true;
                    l = 2*l - 0.5;
                    r = 2*r - 0.5;
                    counter++;
                }
            } while (do_scaling);
            // tmpout << "|-|";
        }
        updateCharOccs(charOccs, block, b);
        // printCharOccs(charOccs);
        allOccs += b;
    }

    tagDouble = (l + r) / 2;
    tmpout.close();
    tmpin.open("temp.cps");
    fout.open(codename);
    fout << n << endl;
    // m = countBuffer(charOccs, n);
    while (getline(tmpin, line)) {
        fout << line << endl;
    }
    // fout << tagDouble << endl;
    fout.close();
    tmpin.close();

    delete block;
    delete charOccs;
    return new ArithmeticCode(n, tagDouble);
}


double bufferToDouble (double value, uchar buffer, int i) {
    i += 8;
    while (buffer > 0) {
        if (buffer % 2 == 1) {
            value += pow(2.0, -i);
        }
        i--;
        buffer /= 2;
    }
    return value;
}

// double shiftBuffer (double value, int i) {
//     return value + pow(2.0, -i);
// }

void updateCharOccs (int* occs, int* tmp) {
    for (int i = 0; i < 256; i++) {
        occs[i] += tmp[i];
        tmp[i] = 0;
    }
}

int pow2 (int e) {
    int value = 1;
    for (; e > 0; e--) {
        value *= 2;
    }
    return value;
}

uchar* decode (int n, uchar* tag, int tagSize, int b) {
    uchar* array = new uchar[n];
    int* charOccs = initCharOccs(1);
    int* tmpOccs = initCharOccs(0);
    // string buffer = "";
    uchar buffer, symbol;
    int bufferSize = 24;
    double bufferTag, l = 0.0, r = 1.0, d;
    int F, allOccs = 256, nextBit, modulo;
    int counter = 0, occsCounter = 0, fileCounter = 0, buffCounter = 0;
    // long long int tagSize = tag.length();
    bool do_scaling = false;
    // double v = pow(2.0, -32.0);
    // cout << "double: " << v << endl;
    bufferTag = 0.0;
    for (fileCounter = 0; 8*fileCounter < bufferSize; fileCounter++) {
        buffer = tag[fileCounter];
        // cout << "bufferr: " << buffer << endl;
        bufferTag = bufferToDouble(bufferTag, buffer, 8*fileCounter);
    }
    buffer = tag[fileCounter++];
    // cout << "bufferrr: " << buffer << endl;
    // cout << "; tag = " << bufferTag << endl;

    for (int i = 0; i < n; i++) {
        symbol = 0;
        d = r - l;
        do {
            symbol++;
            F = sumOfOccs(charOccs, symbol);
        } while (symbol > 0 && l + ((double)F / (double)allOccs) * d <= bufferTag);

        symbol--;
        array[i] = symbol;

        tmpOccs[symbol]++;
        occsCounter++;
        if (occsCounter == b) {
            updateCharOccs(charOccs, tmpOccs);
            occsCounter = 0;
        }

        F = sumOfOccs(charOccs, symbol);
        r = l + (((double)F + (double)charOccs[symbol]) / (double)allOccs) * d;
        l = l + ((double)F / (double)allOccs) * d;

        do {
            do_scaling = false;
            if (r <= 0.5) {
                do_scaling = true;
                l = 2*l;
                r = 2*r;
                bufferTag = 2*bufferTag;
                modulo = buffer % pow2(buffCounter);
                nextBit = buffer - modulo;
                buffer = modulo;
                buffCounter++;
                if (buffCounter == 8) {
                    buffer = fileCounter < tagSize ? tag[fileCounter++] : 0;
                    buffCounter = 0;
                }
                if (nextBit == 1) {
                    bufferTag += pow(2.0, -bufferSize);
                }
                for (; counter > 0; counter--) {
                    modulo = buffer % pow2(buffCounter);
                    nextBit = buffer - modulo;
                    buffer = modulo;
                    buffCounter++;
                    if (buffCounter == 8) {
                        buffer = fileCounter < tagSize ? tag[fileCounter++] : 0;
                        buffCounter = 0;
                    }
                    if (nextBit == 1) {
                        bufferTag += pow(2.0, -bufferSize);
                    }
                }
                counter = 0;
            }
            if (0.5 <= l) {
                do_scaling = true;
                l = 2*l - 1;
                r = 2*r - 1;
                bufferTag = 2*bufferTag - 1;
                modulo = buffer % pow2(buffCounter);
                nextBit = buffer - modulo;
                buffer = modulo;
                buffCounter++;
                if (buffCounter == 8) {
                    buffer = fileCounter < tagSize ? tag[fileCounter++] : 0;
                    buffCounter = 0;
                }
                if (nextBit == 1) {
                    bufferTag += pow(2.0, -bufferSize);
                }
                for (; counter > 0; counter--) {
                    modulo = buffer % pow2(buffCounter);
                    nextBit = buffer - modulo;
                    buffer = modulo;
                    buffCounter++;
                    if (buffCounter == 8) {
                        buffer = fileCounter < tagSize ? tag[fileCounter++] : 0;
                        buffCounter = 0;
                    }
                    if (nextBit == 1) {
                        bufferTag += pow(2.0, -bufferSize);
                    }
                }
                counter = 0;
            }
            if (l < 0.5 && 0.5 < r && 0.25 <= l && r <= 0.75) {
                do_scaling = true;
                l = 2*l - 0.5;
                r = 2*r - 0.5;
                bufferTag = 2*bufferTag - 0.5;
                counter++;
            }
        } while (do_scaling);
    }
    cerr << endl;
    return array;
}

double CR (int nieskompresowany, int skompresowany) {
    return 100.0 * (1.0 - ((double)skompresowany / (double)nieskompresowany));
}


void compress (string filename, string codename) {
    int b = BLOCK_SIZE;
    std::cout << "KOMPRESJA PLIKU " << filename << " --> " << codename << endl;
    ofstream fout;
    int n;
    uchar* array = fileToArray(filename, n);

    auto start = chrono::steady_clock::now();
    ArithmeticCode *code = encode(array, n, b, codename);
    auto end = chrono::steady_clock::now();
    std::cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    std::cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    int skompresowany = 0;
    uchar* array_skompresowany = fileToArray(codename, skompresowany);
    std::cout << "Wskaznik kompresji: " << CR(n, skompresowany) << endl;

    delete array;
    std::cout << endl;
    delete code;
}

void decompress (string codename, string filename) {
    int b = BLOCK_SIZE;
    std::cout << "DEKOMPRESJA " << codename << " DO PLIKU " << filename << endl;
    int n, m = 0;
    // fdec tag;
    string tag;
    uchar* array;
    ifstream fin;
    ofstream fout;
    fin.open(codename);
    fin >> n;
    fin >> tag;
    fin.close();
    uchar* tagArray = fileToArray("temp.cps", m);
    // cout << "n = " << n << "; tag = " << tag << endl;

    auto start = chrono::steady_clock::now();
    array = decode(n, tagArray, m, b);
    auto end = chrono::steady_clock::now();
    std::cout << "Czas dekompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    std::cout << "Czas dekompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;

    fout.open(filename, ios::out|ios::binary);
    fout.write((char *)array, n);
    // for (int i = 0; i < n; i++) {
    //     fout.put((char)array[i]);
    // }
    fout.close();
    delete array;
    delete tagArray;
    std::cout << endl;
}

bool compareFiles (string file1, string file2) {
    int n, n2;
    uchar* array1 = fileToArray(file1, n);
    uchar* array2 = fileToArray(file2, n2);
    int i = 0;
    bool equal = true;

    if (n != n2) {
        std::cout << "ERROR: Rozny rozmiar plikow." << endl;
        equal = false;
    }
    else {
        while (i < n && array1[i] == array2[i]) {
            i++;
        }
        if (i < n && array1[i] != array2[i]) {
            std::cout << "ERROR: Znaleziono rozne znaki na pozycji " << i << endl;
            equal = false;
        }
        // else cout << "SUCCESS - Pliki sa takie same!" << endl;
    }

    delete array1;
    delete array2;
    return equal;
}
