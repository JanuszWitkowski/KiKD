#include <iostream>
#include <map>
#include <vector>

#include "entropy.hh"
#include "huff.hh"

using namespace std;

// typedef unsigned char uchar;
typedef uint8_t uchar;

int main (int argc, char** argv) {
    string filename = "", newname = "newfile";
    // char* codename = "kikd.kkd";
    string codename = "kikd.kkd";
    if (argc >= 2) {
        cerr << "!! 1" << endl;
        filename = argv[1];
        codename = codename;
        cerr << "!! 2" << endl;
        int n = 0;
        uchar* array = fileToArray(filename, n);
        cerr << "!! 3" << endl;
        int* occs = countCharOccs(array, n);
        cerr << "!! 4" << endl;
        map<uchar, string> dict1 = createDict(occs);
        cerr << "!! 5" << endl;
        map<string, uchar> dict2 = encode(array, n, dict1, codename);
        cerr << "!! 6" << endl;
        // decompress(codename, newname);
        // cout << compareFiles(filename, filename) << endl;

        delete occs;
        cerr << "!! 7" << endl;
    }
    cerr << "!! 8" << endl;
    return 0;
}
