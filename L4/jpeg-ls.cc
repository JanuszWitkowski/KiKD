#include "jpeg-ls.hh"

uint8_t** BGRToPredictionCodes (uint8_t*** array, size_t width, size_t height, uint8_t (*pred)(uint8_t***, size_t, size_t, ColorBGRType)) {
    size_t n = width*height;
    uint8_t** codes = new uint8_t*[n];
    // for (size_t i )
}


uint8_t pred0 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    return array[i][j][type];
}

uint8_t pred1 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (j > 0) return array[i][j-1][type];
    return 0;
}

uint8_t pred2 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0) return array[i-1][j][type];
    return 0;
}

uint8_t pred3 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0) return array[i-1][j-1][type];
    return 0;
}

uint8_t pred4 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0)
        return (array[i-1][j][type] + array[i][j-1][type] - array[i-1][j-1][type])%256;
    return 0;
}

uint8_t pred5 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0)
        return (array[i-1][j][type] + (array[i][j-1][type] - array[i-1][j-1][type])/2)%256;
    return 0;
}

uint8_t pred6 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0)
        return (array[i][j-1][type] + (array[i-1][j][type] - array[i-1][j-1][type])/2)%256;
    return 0;
}

uint8_t pred7 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0)
        return ((array[i-1][j][type] + array[i][j-1][type])/2)%256;
    return 0;
}

uint8_t pred8 (uint8_t*** array, size_t i, size_t j, ColorBGRType type) {
    if (i > 0 and j > 0) {
        uint8_t N = array[i-1][j][type];
        uint8_t W = array[i][j-1][type];
        uint8_t NW = array[i-1][j-1][type];
        if (NW >= W and NW >= N)
            return W < N ? W : N;
        else if (NW <= W and NW <= N)
            return W > N ? W : N;
        else return (W + N - NW)%256;
    }
    return 0;
}
