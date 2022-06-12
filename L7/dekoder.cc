#include <iostream>
#include "consts.hh"
#include "bitrw.hh"

size_t dekoder (string inName, string outName) {
    BitReader reader(inName);
    BitWriter writer(outName);
    uchar* reverseHamming = reverseCycleHamming();
    size_t counter = 0;

    //

    delete[] reverseHamming;
    return counter;
}

int main (int argc, char* argv[]) {
    // cout << cGreen << "-----SZUM-----" << cReset << endl;
    if (argc != 3) {
        printError(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<nazwa pliku wejściowego> <nazwa pliku wyjściowego>");
        return 1;
    }
    string in = argv[1], out = argv[2];
    size_t counter = dekoder(in, out);
    cout << cBlue << "Napotkano 2 błędy w " << counter << " przypadkach." << cReset << endl;
    return 0;
}
