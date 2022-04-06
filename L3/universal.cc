#include <iostream>
#include <string>
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;

string decToBin (int x) {
    string b = "";
    while (x > 0) {
        b = (x%2==1 ? "1" : "0") + b;
        x = x >> 1;
    }
    return b;
}

string eliasGamma (int x) {
    string b = decToBin(x);
    int n = b.length();
    string s = "";
    for (; n > 1; n--)
        s = "0" + s;
    return s + b;
}

string eliasDelta (int x) {
    string b = decToBin(x);
    int n = b.length();
    string g = eliasGamma(n);
    int k = g.length();
    string s = "";
    for (; k > 1; k--) {
        s = "0" + s;
    }
    return s + g + b.substr(1, n-1);
}
