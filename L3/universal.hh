#ifndef __UNIVERSAL_H
#define __UNIVERSAL_H

#include <iostream>
#include <string>
#include "bitrw.hh"
using namespace std;
typedef unsigned int uint;

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

#endif
