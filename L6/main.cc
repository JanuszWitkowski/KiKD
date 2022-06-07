#include <iostream>
#include <cstring>
#include "consts.hh"
#include "main.hh"

using namespace std;

bool checkArguments(int c, char* v[], string &arg1, string &arg2, size_t &arg3) {
    if (c < 3 or c > 4) {
        printError("ARGUMENT", "Niepoprawna liczba argumentów.\nPrawidłowe wywołania programu:\n\
                    ./encode <plik wejściowy> <plik wyjściowy> <liczba bitów kwantyzatora [1, 7]>\n\
                    <plik wejściowy> <liczba bitów kwantyzatora [1, 7]>");
        return false;
    }
    string inName = "testy/example0.tga", outName = "output/kikd.kkd";
    size_t qBits = 3;
    inName = v[1];
    if (c == 3) {
        qBits = atof(v[2]);
    } else {
        outName = v[2];
        qBits = atof(v[3]);
    }
    if (qBits < 1 or qBits > 7) {
        printError("QUANTIZER", "Nieprawidłowa liczba bitów kwantyzatora (powinna być z zakesu {1, 2, ..., 7}).");
        return false;
    }
    arg1 = inName;
    arg2 = outName;
    arg3 = qBits;
    return true;
}

void encodeTGA(string inName, string outName, size_t qBits) {
    //
}

void decodeTGA(string inName, string outName) {
    //
    string strCreate = "python3 output/tmp.txt" + outName, strDelete = "rm output/tmp.txt";
    char cmdCreate[strCreate.size()], cmdDelete[strDelete.size()];
    strcpy(cmdCreate, strCreate.c_str());
    strcpy(cmdDelete, strDelete.c_str());
    // THIS BELOW IS CHEATING and I'm not sorry
    system(cmdCreate);
    system(cmdDelete);
}

void compareImages(std::string filename1, std::string filename2) {
    //
}


int main (int argc, char* argv[]) {
    string inName, outName, newName = "output/example.tga";
    size_t qBits;
    if (!checkArguments(argc, argv, inName, outName, qBits)) return 1;
    encodeTGA(inName, outName, qBits);
    decodeTGA(outName, newName);
    return 0;
}
