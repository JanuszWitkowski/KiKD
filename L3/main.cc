#include <iostream>
#include "universal.hh"
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

int main () {
    uint n = 654;
    string g = eliasGamma(n);
    string d = eliasDelta(n);
    string o = eliasOmega(n);
    uint ng = eliasGamma(g);
    uint nd = eliasDelta(d);
    uint no = eliasOmega(o);
    cout << n << "\n";
    cout << g << " == " << ng << "\n";
    cout << d << " == " << nd << "\n";
    cout << o << " == " << no << "\n";
    return 0;
}
