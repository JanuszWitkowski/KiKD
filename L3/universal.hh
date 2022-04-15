#ifndef __UNIVERSAL_H
#define __UNIVERSAL_H

#include <iostream>
#include <string>
#include "bitrw.hh"
using namespace std;
typedef unsigned int uint;

class List {
public:
    uint value;
    List* prev;
    List(uint v, List* p);
    virtual ~List() {
        if (prev != nullptr) delete prev;
    }
};

// SINGLE-USE TESTING METHODS
string eliasGamma (uint x);
uint eliasGamma (string s);

string eliasDelta (uint x);
uint eliasDelta (string s);

string eliasOmega (uint x);
uint eliasOmega (string s);

string fibonacci (uint x);
uint fibonacci (string s);

// FILE-STREAM METHODS NEEDED FOR COMPLETING THE TASK
void eliasGamma (uint x, BitWriter* writer);
uint eliasGamma (BitReader* reader);

void eliasDelta (uint x, BitWriter* writer);
uint eliasDelta (BitReader* reader);

void eliasOmega (uint x, BitWriter* writer);
uint eliasOmega (BitReader* reader);

void fibonacci (uint x, BitWriter* writer);
uint fibonacci (BitReader* reader);

#endif
