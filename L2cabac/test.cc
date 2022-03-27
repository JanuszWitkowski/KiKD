#include <iostream>
#include <limits>
#include <math.h>
#include <chrono>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using boost::multiprecision::cpp_dec_float_100;

int main () {
    cpp_dec_float_100 dec = 2;
    // for (int i = 0; i < 100; i++) {
    //     dec = dec / 2;
    //     cout << fixed << setprecision(100) << dec << endl;
    // }
    dec = 1.6 * dec;
    cout << dec << endl;
    return 0;
}