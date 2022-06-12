#include <iostream>
#include "consts.hh"
#include "bitrw.hh"

using namespace std;

void koder (string inName, string outName) {
    BitReader reader(inName);
    BitWriter writer(outName);
    uchar* hammingCodes = cycleHamming();
    while (!reader.isEOF()) {
        uchar four_bits = 0;
        for (size_t i = 0; i < 4; i++)
            four_bits = (four_bits << 1) + reader.getNextBit();
        writer.writeByte(hammingCodes[four_bits]);
    }
    // writer.padWithZerosByte();
    delete[] hammingCodes;
}

int main (int argc, char* argv[]) {
    // cout << cGreen << "-----SZUM-----" << cReset << endl;
    if (argc != 3) {
        printError(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<nazwa pliku wejściowego> <nazwa pliku wyjściowego>");
        return 1;
    }
    string in = argv[1], out = argv[2];
    koder(in, out);
    return 0;
}
