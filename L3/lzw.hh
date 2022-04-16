#ifndef __LZW_H_
#define __LZW_H_

#include <iostream>
#include <string>
#include "universal.hh"
using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

// string universal_code_names[] = {
//     "Elias Gamma",
//     "Elias Delta",
//     "Elias Omega",
//     "Fibonacci",
//     "Elias Omega (default)"
// };

void LZW_encode (string filename, string codename, UniversalCodingType type);
void LZW_decode (string codename, string filename);

#endif
