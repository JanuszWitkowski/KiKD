#include <iostream>
#include <string>
#include <math.h>
#include "universal.hh"
using namespace std;
typedef unsigned int uint;

// HELPER FUNCTIONS
string decToBin (uint x) {
    string b = "";
    while (x > 0) {
        b = (x%2==1 ? "1" : "0") + b;
        x = x >> 1;
    }
    return b;
}

uint binToDec (string b) {
    uint x = 0, p = 1;
    size_t k = b.length() - 1;
    for (int i = k; i >= 0; i--) {
        if (b[i] == '1') {
            x += p;
        }
        p <<= 1;
    }
    return x;
}


// FILE-STREAM METHODS NEEDED FOR COMPLETING THE TASK
void eliasGamma (uint x, BitWriter* writer) {
    size_t bitIndex = 31;
    while (((x >> bitIndex) & 1) == 0 && bitIndex > 0) {
        bitIndex--;
    }
    // 0 -> 10, 1 -> 11
    if (bitIndex == 0) {
        writer->writeBit(1);
        writer->writeBit(x & 1);
    }
    else {
        size_t n = bitIndex + 1;
        // cout << "Size: " << n;
        for (; n > 1; n--)
            writer->writeBit(0);
        // cout << "; number = ";
        for (; bitIndex > 0; bitIndex--) {
            writer->writeBit((x >> bitIndex) & 1);
            // cout << ((x >> bitIndex) & 1);
        }
        // cout << endl;
        writer->writeBit(x & 1);
    }
}

uint eliasGamma (BitReader* reader) {
    size_t n = 0;
    while (!(reader->isNextBitOne())) {
        n++;
    }
    if (n == 0) {
        if (reader->isNextBitOne()) {
            return 1;
        }
        else return 0;
    }
    uint x = 1;
    for (; n > 0; n--) {
        x = (x << 1) + reader->getNextBit();
    }
    return x;
}


// SINGLE-USE TESTING METHODS
string eliasGamma (uint x) {
    string b = decToBin(x);
    size_t n = b.length();
    string s = "";
    for (; n > 1; n--)
        s = "0" + s;
    return s + b;
}

uint eliasGamma (string s) {
    size_t k = 0;
    while (s[k] == '0') k++;
    return binToDec(s.substr(k, k+1));
}


string eliasDelta (uint x) {
    string b = decToBin(x);
    size_t n = b.length();
    string g = eliasGamma(n);
    size_t k = g.length();
    return g + b.substr(1, n-1);
}

uint eliasDelta (string s) {
    size_t k = 0, n;
    while (s[k] == '0') k++;
    n = binToDec(s.substr(k, k+1));
    return binToDec('1' + s.substr(k+k+1, n-1));
}


string eliasOmega (uint x) {
    string s = "0", b;
    uint k = x;
    while (k > 1) {
        b = decToBin(k);
        s = b + s;
        k = b.length() - 1;
    }
    return s;
}

uint eliasOmega (string s) {
    uint n = 1, prev;
    size_t i = 0;
    while (s[i] != '0') {
        prev = n + 1;
        n = binToDec(s.substr(i, prev));
        i += prev;
    }
    return n;
}
