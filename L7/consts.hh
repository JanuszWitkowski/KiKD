#pragma once

#include <iostream>
#include <string>
#include "consts.hh"

using namespace std;

const string cReset   = "\033[0m";
const string cBlack   = "\033[30m";
const string cRed     = "\033[31m";
const string cGreen   = "\033[32m";
const string cYellow  = "\033[33m";
const string cBlue    = "\033[34m";
const string cMagenta = "\033[35m";
const string cCyan    = "\033[36m";
const string cWhite   = "\033[37m";


enum ErrorType {
    ARGUMENT,
    PARSING,
    RUNTIME,
    MISMATCH
};

const string ErrorTypeName[4] = {
    "ARGUMENT",
    "PARSING",
    "RUNTIME",
    "MISMATCH"
};

void printError(ErrorType type, string msg) {
    cerr << cRed << ErrorTypeName[type] << " ERROR: " << msg << cReset << endl;
}

uint8_t* cycleHamming () {
    uint8_t* h = new uint8_t[16];
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t code = 0;
        uint8_t a = ((i >> 3) & 1);
        uint8_t b = ((i >> 2) & 1);
        uint8_t c = ((i >> 1) & 1);
        uint8_t d = (i & 1);
        code = (a << 7) +
                (((a + b) % 2) << 6) +
                (((b + c) % 2) << 5) +
                (((a + c + d) % 2) << 4) +
                (((b + d) % 2) << 3) +
                (c << 2) +
                (d << 1);
        uint8_t ones_counter = 0;
        for (size_t j = 7; j > 0; j--)
            if ( ((code >> j) & 1) == 1 )
                ones_counter++;
        code += (ones_counter % 2);
        h[i] = code;
    }
    return h;
}
