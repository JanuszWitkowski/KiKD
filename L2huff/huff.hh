#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#include <iostream>
#include <fstream>
#include <map>

#include "entropy.hh"

using namespace std;

// typedef unsigned char uchar;
typedef uint8_t uchar;


typedef struct {
    vector<uchar> symbols;
    int occ;
} Node;

#endif