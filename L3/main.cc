#include <iostream>
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;

int main () {
    int n = 30;
    string g = eliasGamma(n);
    string d = eliasDelta(n);
    cout << g << "\n";
    cout << d << "\n";
    return 0;
}
