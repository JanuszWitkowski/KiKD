#include <iostream>
#include "consts.hh"
#include "bitrw.hh"

uint8_t* reverseCycleHamming () {
    uint8_t* h = cycleHamming();
    uint8_t* r = new uint8_t[256];
    for (size_t i = 0; i < 256; i++)
        r[i] = 16;
    for (uint8_t number = 0; number < 16; number++)
        r[h[number]] = number;
    delete[] h;
    return r;
}

uchar* checkMatrix () {
    uchar* m = new uchar[256];
    uchar i = 0;
    for (size_t j = 0; j < 256; j++) {
        uchar a = ((i >> 7) & 1);
        uchar b = ((i >> 6) & 1);
        uchar c = ((i >> 5) & 1);
        uchar d = ((i >> 4) & 1);
        uchar e = ((i >> 3) & 1);
        uchar f = ((i >> 2) & 1);
        uchar g = ((i >> 1) & 1);
        uchar check = (((c + e + f + g) % 2) << 2) +
                        (((b + d + e + f) % 2) << 1) +
                        ((a + c + d + e) % 2);
        // uchar check = (((a + b + c + d) % 2) << 2) +
        //                 (((a + b + e + f) % 2) << 1) +
        //                 ((a + c + e + g) % 2);
        m[i] = check;
        i++;
    }
    return m;
}
uchar checkToShift[8] = {
    0, 7, 6, 4, 1, 5, 2, 3
};

size_t dekoder (string inName, string outName, size_t &fix_counter) {
    BitReader reader(inName);
    BitWriter writer(outName);
    uchar* reverseHamming = reverseCycleHamming();
    uchar* checkCodes = checkMatrix();
    size_t counter = 0;
    fix_counter = 0;

    while (!reader.isEOF()) {
        uchar byte = reader.getNextByte();
        uchar parity = (byte & 1);
        size_t ones_counter = 0;
        for (size_t i = 7; i > 0; i--)
            ones_counter += ((byte >> i) & 1);
        ones_counter %= 2;
        uchar check = checkCodes[byte];
        uchar to_write = 0;
        if (parity == ones_counter) {   // parzysta liczba błędów
            if (check != 0) {
                counter++;
            }
            to_write = reverseHamming[byte];
        }
        else {      // nieparzysta liczba błędów
            uchar shift = checkToShift[check];
            byte = byte - (((byte >> shift) & 1) << shift) + ((1 - ((byte >> shift) & 1)) << shift);
            to_write = reverseHamming[byte];
            fix_counter++;
        }
        for (int i = 3; i >= 0; i--)
            writer.writeBit((to_write >> i) & 1);
    }

    delete[] reverseHamming;
    delete[] checkCodes;
    return counter;
}

int main (int argc, char* argv[]) {
    cout << cGreen << "-----DEKODER-----" << cReset << endl;
    if (argc != 3) {
        printError(ARGUMENT, "Niepoprawna liczba argumentów. Schemat podawania argumentów:\n<nazwa pliku wejściowego> <nazwa pliku wyjściowego>");
        return 1;
    }
    string in = argv[1], out = argv[2];
    size_t fix_counter, err_counter = dekoder(in, out, fix_counter);
    cout << cBlue << "Napotkano 2 błędy w " << err_counter << " przypadkach. Naprawiono " << fix_counter << " bloków." << cReset << endl;
    return 0;
}
