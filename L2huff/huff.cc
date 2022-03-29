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

FILE* f;

/* how many bits in current byte */
int bit_counter;
/* current byte */
uchar cur_byte;

/* write 1 or 0 bit */
void write_bit(uchar bit) {
    if(++bit_counter == 8) {
        fwrite(&cur_byte,1,1,f);
        bit_counter = 0;
        cur_byte = 0;
    }
    cur_byte <<= 1;
    cur_byte |= bit;
}

map<string, int> encode (uchar* array, int n, int* charOccs) {
    map<string, int> dict;
    vector<uchar> usedChars;
    vector<string> strings;
    vector<Node*> nodes;
    for (int i = 0; i < 256; i++) {
        strings.push_back("");
        if (charOccs[i] > 0) {
            usedChars.push_back(i);
            Node *node;
            node->symbols.push_back(i);
            node->occ = charOccs[i];
            nodes.push_back(node);
        }
    }
    int m = usedChars.size();

    //find 2 smallest
    int minv1 = INT_MAX, minv2 = INT_MAX, tmpv;
    int mini1, mini2, tmpi;
    for (int i = 0; i < m; i++) {
        if (nodes[i]->occ < minv2) {
            minv2 = nodes[i]->occ;
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
    Node *node;
    for (int i = 0; i < nodes[mini1]->symbols.size(); i++) {
        node->symbols.push_back(nodes[mini1]->symbols[i]);
        strings[nodes[mini1]->symbols[i]] = "0" + strings[nodes[mini1]->symbols[i]];
    }
    for (int i = 0; i < nodes[mini2]->symbols.size(); i++) {
        node->symbols.push_back(nodes[mini2]->symbols[i]);
        strings[nodes[mini2]->symbols[i]] = "1" + strings[nodes[mini2]->symbols[i]];
    }
    node->occ = nodes[mini1]->occ + nodes[mini2]->occ;
    nodes[mini1] = node;
    nodes.erase(nodes.begin() + mini2);

    // int m = usedChars.size();
    return dict;
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



