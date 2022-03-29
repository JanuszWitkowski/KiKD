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
// typedef cpp_dec_float_50 fdec;
// typedef cpp_dec_float_100 fdec;
typedef cpp_dec_float_100 bigfloat;
// typedef boost::multiprecision::number<cpp_dec_float<200> > fdec;
typedef double fdec;

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
                    tmpout << 0;
                    for (; counter > 0; counter--) tmpout << 1;
                    counter = 0;
                }
                if (0.5 <= l) {
                    do_scaling = true;
                    l = 2*l - 1;
                    r = 2*r - 1;
                    tmpout << 1;
                    for (; counter > 0; counter--) tmpout << 0;
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

uchar* decode (int n, string tag) {
    uchar* array = new uchar[n];
    string buffer = "";
    int bufferSize = 32;
    double bufferTag, l = 0.0, r = 1.0;
    int symbol;
    int* charOccs = initCharOccs(1);
    // double v = pow(2.0, -32.0);
    // cout << "double: " << v << endl;
    buffer = tag.substr(0, bufferSize);
    bufferTag = stringToDouble(buffer);
    // cout << "buffer = " << buffer << "; tag = " << bufferTag << endl;

    for (int i = 0; i < n; i++) {
        while (false) {
            //
        }
    }

    return array;
}


void compress (string filename, string codename) {
    int b = 256;
    std::cout << "KOMPRESJA PLIKU " << filename << " --> " << codename << endl;
    ofstream fout;
    int n;
    uchar* array = fileToArray(filename, n);

    auto start = chrono::steady_clock::now();
    ArithmeticCode *code = encode(array, n, b, codename);
    auto end = chrono::steady_clock::now();
    std::cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    std::cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;

    delete array;
    std::cout << endl;
    delete code;
}

void decompress (string codename, string filename) {
    std::cout << "DEKOMPRESJA " << codename << " DO PLIKU " << filename << endl;
    int n;
    // fdec tag;
    string tag;
    uchar* array;
    ifstream fin;
    ofstream fout;
    fin.open(codename);
    fin >> n;
    fin >> tag;
    fin.close();
    // cout << "n = " << n << "; tag = " << tag << endl;

    auto start = chrono::steady_clock::now();
    array = decode(n, tag);
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
