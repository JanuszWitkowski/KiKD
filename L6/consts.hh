#pragma once

#include <string>

using namespace std;

typedef uint8_t uchar;

enum ColorBGRType {
    BLUE,
    GREEN,
    RED,
    ALL
};

const string cReset   = "\033[0m";
const string cBlack   = "\033[30m";
const string cRed     = "\033[31m";
const string cGreen   = "\033[32m";
const string cYellow  = "\033[33m";
const string cBlue    = "\033[34m";
const string cMagenta = "\033[35m";
const string cCyan    = "\033[36m";
const string cWhite   = "\033[37m";

void printError(string errorType, string errorMsg) {
    cout << cRed << errorType << " ERROR: ";
    cout << errorMsg << cReset << endl;
}
