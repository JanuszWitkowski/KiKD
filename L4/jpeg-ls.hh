#ifndef __JPEGLS_H
#define __JPEGLS_H

#include <iostream>
#include "tga.hh"

using namespace std;

uint8_t** BGRToPredictionCodes (uint8_t*** array, size_t width, size_t height, uint8_t (*pred)(uint8_t***, size_t, size_t, ColorBGRType));

uint8_t pred0 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred1 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred2 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred3 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred4 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred5 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred6 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred7 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);
uint8_t pred8 (uint8_t*** array, size_t i, size_t j, ColorBGRType type);

#endif