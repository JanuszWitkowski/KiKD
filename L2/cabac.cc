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
typedef cpp_dec_float_100 fdec;
// typedef boost::multiprecision::number<cpp_dec_float<200> > fdec;

ArithmeticCode::ArithmeticCode(int length, fdec value) {
    setN(length);
    setTag(value);
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

// void encodeBlock (uchar* block) {

// }

ArithmeticCode *encode (uchar* array, int n, int b) {
    fdec tag = 0;
    fdec l = 0, r = 1, d;
    int F;
    uchar* block = new uchar[b];
    uchar symbol;
    int* charOccs = initCharOccs(1);    // ustaw liczbe wystapien kazdego znaku na 1
    int allOccs = 256;
    string scale = "";      // co to znaczy "dolacz do KODU slowo 01^licznik" ???
    int counter = 0;

    for (int i = 0; i < n; i += b) {
        block = setBlock(array, block, i, b, n);
        for (int j = 0; j < b; j++) {
            symbol = block[j];
            F = sumOfOccs(charOccs, symbol);
            d = r - l;
            r = l + (((double)F + (double)charOccs[symbol]) / (double)allOccs) * d;     // dzielenie!!!
            l = l + ((double)F / (double)allOccs) * d;                          // dzielenie!!!
            // cout << "it: " << i << "; [" << l << ", " << r << ")" << endl;
            if (i == 0) cout << "it: " << i + j << "; [" << fixed << setprecision(100) << l << ", " << r << ")" << endl;
            while (r <= 0.5) {
                l = 2*l;
                r = 2*r;
                //
                counter = 0;
            }
            while (0.5 <= l) {
                l = 2*l - 1;
                r = 2*r - 1;
                //
                counter = 0;
            }
            while (l < 0.5 && 0.5 < r && 0.25 <= l && r <= 0.75) {
                l = 2*l - 0.5;
                r = 2*r - 0.5;
                counter++;
            }
            // if (l == r) cout << "!!!equal" << endl;
        }
        updateCharOccs(charOccs, block, b);
        allOccs += b;
    }

    delete block;
    delete charOccs;

    tag = (l + r) / 2;
    return new ArithmeticCode(n, tag);
}

uchar* decode (int n, double tag) {
    uchar* array = new uchar[n];
    return array;
}


void compress (string filename, string codename) {
    int b = 64;
    cout << "KOMPRESJA PLIKU " << filename << " --> " << codename << endl;
    ofstream fout;
    int n;
    uchar* array = fileToArray(filename, n);

    auto start = chrono::steady_clock::now();
    ArithmeticCode *code = encode(array, n, b);
    auto end = chrono::steady_clock::now();
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "Znacznik: " << fixed << setprecision(100) << code->getTag() << endl;

    delete array;
    fout.open(codename);
    fout << code->getN() << endl;
    fout << fixed << setprecision(100) << code->getTag() << endl;
    fout.close();
    cout << endl;
}

void decompress (string codename, string filename) {
    cout << "DEKOMPRESJA " << codename << " DO PLIKU " << filename << endl;
    int n;
    double tag;
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
    cout << "Czas dekompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;
    cout << "Czas dekompresji: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;

    fout.open(filename, ios::out|ios::binary);
    fout.write((char *)array, n);
    // for (int i = 0; i < n; i++) {
    //     fout.put((char)array[i]);
    // }
    fout.close();
    delete array;
    cout << endl;
}

bool compareFiles (string file1, string file2) {
    int n, n2;
    uchar* array1 = fileToArray(file1, n);
    uchar* array2 = fileToArray(file2, n2);
    int i = 0;
    bool equal = true;

    if (n != n2) {
        cout << "ERROR: Rozny rozmiar plikow." << endl;
        equal = false;
    }
    else {
        while (i < n && array1[i] == array2[i]) {
            i++;
        }
        if (i < n && array1[i] != array2[i]) {
            cout << "ERROR: Znaleziono rozne znaki na pozycji " << i << endl;
            equal = false;
        }
        // else cout << "SUCCESS - Pliki sa takie same!" << endl;
    }

    delete array1;
    delete array2;
    return equal;
}
