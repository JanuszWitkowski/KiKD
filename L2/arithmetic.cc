#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <chrono>

#include "entropy.hh"
#include "arithmetic.hh"
using namespace std;

typedef unsigned char uchar;

ArithmeticCode::ArithmeticCode(int length, double value) {
    n(length);
    tag(value);
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

double sumOfOccs (int* occs, uchar symbol) {
    double sum = 0.0;
    for (int i = symbol - 1; i >= 0; i--) {
        sum += occs[i];
    }
    return sum;
}

// void encodeBlock (uchar* block) {

// }

ArithmeticCode *encode (uchar* array, int n) {
    double tag = 0.0;
    double l = 0.0, r = 1.0, d, F;
    uchar* block = new uchar[256];
    uchar symbol;
    int* charOccs = initCharOccs(1);    // ustaw liczbe wystapien kazdego znaku na 1
    double allOccs = 256.0;

    for (int i = 0; i < n; i += 256) {
        block = setBlock(array, block, i, 256, n);
        for (int j = 0; j < 256; j++) {
            symbol = block[j];
            F = sumOfOccs(charOccs, symbol);
            d = r - l;
            r = l + ((F + charOccs[symbol]) / allOccs) * d;     // dzielenie!!!
            l = l + (F / allOccs) * d;                          // dzielenie!!!
        }
        updateCharOccs(charOccs, block, 256);
        allOccs += 256.0;
    }

    delete block;
    delete charOccs;

    tag = (l + r) / 2.0;
    return new ArithmeticCode(n, tag);
}

uchar* decode (int n, double tag) {
    uchar* array = new uchar[n];
    return array;
}


void compress (string filename, string codename) {
    ofstream fout;
    uchar* array = fileToArray(filename);
    int n = sizeof(array) / sizeof(array[0]);

    auto start = chrono::steady_clock::now();
    ArithmeticCode *code = encode(array, n);
    auto end = chrono::steady_clock::now();
    cout << "Czas kompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;

    delete array;
    fout.open(codename);
    fout << code->n() << endl;
    fout << code->tag() << endl;
    fout.close();
}

void decompress (string codename, string filename) {
    int n;
    double tag;
    uchar* array;
    ifstream fin;
    ofstream fout;
    fin.open(codename);
    fin >> n;
    fin >> tag;
    fin.close();

    auto start = chrono::steady_clock::now();
    array = decode(n, tag);
    auto end = chrono::steady_clock::now();
    cout << "Czas dekompresji: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;

    fout.open(filename);
    for (int i = 0; i < n; i++) {
        fout << array[i];
    }
    delete array;
    fout.close();
}

void compareFiles (string file1, string file2) {
    uchar* array1 = fileToArray(file1);
    uchar* array2 = fileToArray(file2);
    int n = sizeof(array1) / sizeof(array1[0]);
    int i = 0;

    if (n != sizeof(array2) / sizeof(array2[0])) {
        cout << "ERROR: Rozny rozmiar plikow." << endl;
    }
    else {
        while (i < n && array1[i] == array2[i]) {
            i++;
        }
        if (i < n && array1[i] != array2[i]) {
            cout << "ERROR: Znaleziono rozne znaki na pozycji " << i << endl;
        }
        else cout << "SUCCESS - PLiki sa takie same!" << endl;
    }

    delete array1;
    delete array2;
}
