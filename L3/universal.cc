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

List::List(uint v, List* p) {
    value = v;
    prev = p;
}


// FILE-STREAM METHODS NEEDED FOR COMPLETING THE TASK
void eliasGamma (uint x, BitWriter* writer) {
    if (x <= 1) {
        // 0 -> 10, 1 -> 11
        writer->writeBit(1);
        writer->writeBit(x & 1);
    }
    else {
        size_t bitIndex = 31;
        while (((x >> bitIndex) & 1) == 0 && bitIndex > 0) {
            bitIndex--;
        }
        size_t n = bitIndex + 1;
        for (; n > 1; n--)
            writer->writeBit(0);
        for (; bitIndex > 0; bitIndex--) {
            writer->writeBit((x >> bitIndex) & 1);
        }
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


void eliasDelta (uint x, BitWriter* writer) {
    if (x <= 1) {
        // 0 -> 10, 1 -> 11
        writer->writeBit(1);
        writer->writeBit(x & 1);
    }
    else {
        size_t xBitIndex = 31;
        while (((x >> xBitIndex) & 1) == 0 && xBitIndex > 0) {
            xBitIndex--;
        }
        size_t n = xBitIndex + 1;
        size_t nBitIndex = 31;
        while (((n >> nBitIndex) & 1) == 0 && nBitIndex > 0) {
            nBitIndex--;
        }
        size_t k = nBitIndex + 1;
        for (; k > 1; k--) {
            writer->writeBit(0);
        }
        for (; nBitIndex > 0; nBitIndex--) {
            writer->writeBit((n >> nBitIndex) & 1);
        }
        writer->writeBit(n & 1);
        for (; xBitIndex > 0; xBitIndex--) {
            writer->writeBit((x >> xBitIndex) & 1);
        }
        writer->writeBit(x & 1);
    }
}

uint eliasDelta (BitReader* reader) {
    size_t k = 0;
    while (!(reader->isNextBitOne())) {
        k++;
    }
    if (k == 0) {
        if (reader->isNextBitOne()) {
            return 1;
        }
        else return 0;
    }
    size_t n = 1;
    for (; k > 0; k--) {
        n = (n << 1) + reader->getNextBit();
    }
    uint x = 0;
    for (; n > 0; n--) {
        x = (x << 1) + reader->getNextBit();
    }
    return x;
}


void eliasOmega (uint x, BitWriter* writer) {
    if (x <= 1) {
        writer->writeBit(0);
        writer->writeBit(x & 1);
    }
    else {
        uint k = x;
        List* current = nullptr;
        while (k > 1) {
            List* next = new List(k, current);
            current = next;
            size_t bitIndex = 31;
            while (((k >> bitIndex) & 1) == 0 && bitIndex > 0) {
                bitIndex--;
            }
            k = bitIndex;
        }
        List* list = current;
        while (current != nullptr) {
            size_t bitIndex = 31;
            while (((current->value >> bitIndex) & 1) == 0 && bitIndex > 0) {
                bitIndex--;
            }
            while (bitIndex > 0) {
                writer->writeBit((current->value >> bitIndex) & 1);
                bitIndex--;
            }
            writer->writeBit((current->value) & 1);
            current = current->prev;
        }
        writer->writeBit(0);
        delete list;
    }
}

uint eliasOmega (BitReader* reader) {
    uint n = 1, length;
    while (reader->isNextBitOne()) {
        length = n;
        n = 1;
        for (size_t i = 0; i < length; i++) {
            n = (n << 1) + reader->getNextBit();
        }
    }
    if (n == 1) {
        if (reader->isNextBitOne())
            return 1;
        return 0;
    }
    return n;
}


void fibonacci (uint x, BitWriter* writer) {
    //
}

uint fibonacci (BitReader* reader) {
    uint x = 0;
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
