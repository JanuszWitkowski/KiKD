#include <iostream>

#include "entropy.hh"
#include "cabac.hh"

using namespace std;

int main (int argc, char** argv) {
    string filename = "", codename = "cmprss.cps", newname = "newfile.txt";
    if (argc >= 2) {
        filename = argv[1];
        codename = codename;
        compress(filename, codename);
        decompress(codename, newname);
    }
    return 0;
}
