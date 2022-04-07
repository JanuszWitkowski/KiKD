#include <iostream>
#include <string>
#include "universal.hh"
using namespace std;
typedef unsigned int uint;

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
        p *= 2;
    }
    return x;
}

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
