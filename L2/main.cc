#include <iostream>

#include "entropy.hh"
#include "cabac.hh"

using namespace std;

int main (int argc, char** argv) {
    // cout << "MAIN" << endl;
    string filename = "", codename = "compressed/cmprss.cps";
    // cout << "argv[0] == " << argv[0] << endl;
    if (argc >= 2) {
        filename = argv[1];
        codename = codename;
        // cout << "argv[1] == " << argv[1] << endl;
        compress(filename, codename);
    }
    return 0;
}
