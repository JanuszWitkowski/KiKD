#ifndef __LZW_H_
#define __LZW_H_

#include <iostream>
#include <string>
#include "universal.hh"
using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

void encode (string filename, string codename, UniversalCodingType type);
void decode (string codename, string filename);

#endif
