#include <iostream>
#include <memory>
#include "bitrw.hh"
#include "consts.hh"

using namespace std;

size_t sprawdz(string in1, string in2) {
    unique_ptr<BitReader> reader1 = make_unique<BitReader>(in1);
    unique_ptr<BitReader> reader2 = make_unique<BitReader>(in2);
    size_t size1 = reader1->getFileSizeInBits(), size2 = reader2->getFileSizeInBits();
    uint bits1[4], bits2[4];
    size_t counter = 0, i = 0;
    while (i < size1 && i < size2) {
        for (size_t j = 0; j < 4; j++) {
            bits1[j] = reader1->getNextBit();
            bits2[j] = reader2->getNextBit();
        }
        for (size_t j = 0; j < 4; j++) {
            if (bits1[j] != bits2[j]) {
                counter++;
                break;
            }
        }
        i += 4;
    }
    if (i < size1 || i < size2) {
        size_t howManyLeft = (size1 < size2 ? size2-size1 : size1-size2) / 4;
        string msg = "Pozostało " + to_string(howManyLeft) + " niesprawdzonych bloków (różna długość plików).";
        printError(MISMATCH, msg);
    }
    return counter;
}

int main(int argc, char* argv[]) {
    cout << cGreen << "----SPRAWDZ----" << cReset << endl;
    if (argc != 3) {
        printError(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<nazwa pierwszego pliku> <nazwa drugiego pliku>");
        return 1;
    }
    string in1 = argv[1], in2 = argv[2];
    cout << cGreen << "PODANE PARAMETRY\nInput1: " << cYellow << in1 << cGreen << "\nInput2: " << cYellow << in2 << cReset << endl;
    size_t counter = sprawdz(in1, in2);
    cout << cBlue << "Znaleziono " << counter << " niezgodnych 4-bitowych bloków." << cReset << endl;
    return 0;
}
