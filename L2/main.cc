#include <iostream>

#include "entropy.hh"
#include "cabac.hh"

using namespace std;

int main (int argc, char* argv) {
    string filename = "";
    if (argc >= 2) {
        compress(filename, "compressed-" + filename);
    }
    return 0;
}
