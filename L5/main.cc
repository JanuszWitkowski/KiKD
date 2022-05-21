#include <iostream>
#include "pixel.hh"
#include "bitrw.hh"
#include "tga.hh"
#include "stats.hh"

using namespace std;

int main (int argc, char* argv[]) {
    if (argc != 4) {
        cout << "BŁĄD: Zła liczba argumentów." << endl;
        cout << "Wywołanie programu powinno wyglądać następująco:" << endl;
        cout << "./main [nazwa pliku wejściowego] [nazwa pliku wyjściowego] [wykładnik liczby kolorów 0-24]" << endl;
    } else {
        string inName = argv[1], outName = argv[2];
        int colorNumberExp = stoi(argv[3]);
        if  (colorNumberExp < 0 || colorNumberExp > 24) {
            cout << "BŁĄD: Niepoprawny parametr liczby kolorów." << endl;
            cout << "Dany parametr powinien przyjąć liczbę całkowitą z zakresu [0, 24]." << endl;
        } else {
            //
        }
    }
    return 0;
}
