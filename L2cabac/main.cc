#include <iostream>

#include "entropy.hh"
#include "cabac.hh"

using namespace std;

int main (int argc, char** argv) {
    string filename = "", codename = "kikd.kkd", newname = "newfile.txt";
    if (argc >= 2) {
        filename = argv[1];
        codename = codename;
        compress(filename, codename);
        decompress(codename, newname);
        cout << compareFiles(filename, newname) << endl;
    }
    return 0;
}
