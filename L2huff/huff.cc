#include <iostream>
#include <stdio.h>
#include <fstream>
#include <map>
#include <vector>
#include <climits>

#include "entropy.hh"
#include "huff.hh"

using namespace std;

// typedef unsigned char uchar;
typedef uint8_t uchar;


map<uchar, string> createDict (int* charOccs) {
    cerr << "!!!! 0" << endl;
    map<uchar, string> dict;
    vector<uchar> usedChars;
    vector<string> strings;
    vector<Node> nodes;
    cerr << "!!!! 0.1" << endl;
    for (int i = 0; i < 256; i++) {
        cerr << "!!!! 0.11: " << i << endl;
        strings.push_back("");
        cerr << "!!!!!! 0.111: " << i << endl;
        if (charOccs[i] > 0) {
            cerr << "!!!!!! 0.112: " << i << endl;
            usedChars.push_back(i);
            cerr << "!!!!!! 0.113: " << i << endl;
            Node node = {{}, charOccs[i]};
            cerr << "!!!!!! 0.114: " << i << endl;
            // node->symbols = new vector<>();
            node.symbols.push_back(i);
            cerr << "!!!!!! 0.115: " << i << endl;
            node.occ = charOccs[i];
            cerr << "!!!!!! 0.116: " << i << endl;
            nodes.push_back(node);
            cerr << "!!!!!! 0.117: " << i << endl;
        }
        cerr << "!!!! 0.19: " << i << endl;
    }
    int m = usedChars.size();
    cerr << "!!!! 1" << endl;

    for (int j = 0; j < m-1; j++) {     // for
    //find 2 smallest
    int minv1 = INT_MAX, minv2 = INT_MAX, tmpv;
    int mini1, mini2, tmpi;
    for (int i = 0; i < m; i++) {
        if (nodes[i].occ < minv2) {
            minv2 = nodes[i].occ;
            mini2 = i;
            if (minv2 < minv1) {
                tmpv = minv1;
                minv1 = minv2;
                minv2 = tmpv;
                tmpi = mini1;
                mini1 = mini2;
                mini2 = tmpi;
            }
        }
    }
    cerr << "!!!! 2" << endl;
    Node node;
    // Node tmp1, tmp2;
    for (int i = 0; i < nodes[mini1].symbols.size(); i++) {
        node.symbols.push_back(nodes[mini1].symbols[i]);
        strings[nodes[mini1].symbols[i]] = "0" + strings[nodes[mini1].symbols[i]];
    }
    for (int i = 0; i < nodes[mini2].symbols.size(); i++) {
        node.symbols.push_back(nodes[mini2].symbols[i]);
        strings[nodes[mini2].symbols[i]] = "1" + strings[nodes[mini2].symbols[i]];
    }
    node.occ = nodes[mini1].occ + nodes[mini2].occ;
    delete nodes[mini1].symbols;
    nodes[mini1] = node;
    nodes.erase(nodes.begin() + mini2);

    }   // end for
    cerr << "!!!! 3" << endl;

    // uzupelnienie slownika
    for (int j = 0; j < m; j++) {
        dict.insert({ usedChars[j], strings[usedChars[j]] });
    }
    cerr << "!!!! 4" << endl;

    return dict;
}

map<string, uchar> encode (uchar* array, int n, map<uchar, string> dict, string codename) {
    cerr << "!!!!!! 1" << endl;
    // FILE* f = fopen(codename, "w");       // kikd.kkd
    ofstream fout;
    fout.open(codename, ios::out|ios::binary);
    cerr << "!!!!!! 2" << endl;
    uchar symbol, bit;
    int bit_counter = 0;
    uchar cur_byte = 0;
    string code;
    int size;
    cerr << "!!!!!! 3" << endl;
    for (int i = 0; i < n; i++) {
        cerr << "!!!!!! 3.1: " << i << endl;
        code = dict.at(array[i]);
        cerr << "!!!!!! 3.2: " << i << endl;
        size = code.length();
        cerr << "!!!!!! 3.3: " << i << endl;
        for (int j = 0; j < size; j++) {
            // write_bit(code.at(j));
            cerr << "!!!!!!!! 3.3.1" << endl;
            if(++bit_counter == 8) {
                cerr << "!!!!!!!! 3.3.2" << endl;
                // fwrite(&cur_byte,1,1,f);
                fout << cur_byte;
                cerr << "!!!!!!!! 3.3.3" << endl;
                bit_counter = 0;
                cerr << "!!!!!!!! 3.3.4" << endl;
                cur_byte = 0;
                cerr << "!!!!!!!! 3.3.5" << endl;
            }
            cerr << "!!!!!!!! 3.3.6" << endl;
            // cur_byte <<= 1;
            cur_byte *= 2;
            cerr << "!!!!!!!! 3.3.7" << endl;
            // cur_byte |= bit;
            cur_byte += code.at(j);
            cerr << "!!!!!!!! 3.3.8" << endl;
        }
        cerr << "!!!!!! 3.4: " << i << endl;
    }
    if(bit_counter > 0) {
         // pad the last byte with zeroes
         cur_byte <<= 8 - bit_counter;
        //  fwrite(&cur_byte, 1, 1, f);
        fout << cur_byte;
    }
    // fclose(f);
    fout.close();
    cerr << "!!!!!! 4" << endl;

    // create new dict
    map<string, uchar> new_dict;
    cerr << "!!!!!! 5" << endl;
    for (auto const& x : dict) {
        new_dict.insert({x.second, x.first});
    }
    cerr << "!!!!!! 6" << endl;
    return new_dict;
}

// int main()
// {
//     f = fopen("test.bits", "w");

//     cur_byte = 0;
//     bit_counter = 0;

//     /* write the number of bits here to decode the bitstream later (47 in your case) */
//     /* int num = 47; */           
//     /* fwrite(num, 1, 4, f); */

//     write_bit(1);
//     write_bit(0);
//     write_bit(0);
//     /* etc...  - do this in a loop for each encoded character */
//     /* 100110000100111010011111000010110110111011011111001010 */

//     if(bit_counter > 0)
//     {
//          // pad the last byte with zeroes
//          cur_byte <<= 8 - bit_counter;
//          fwrite(&cur_byte, 1, 1, f);
//     }

//     fclose(f);

//     return 0;
// }



