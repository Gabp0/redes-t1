#include "checksum.h"
#include "convolution.h"
#include "../utils/bits.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    string buffer;
    getline(cin, buffer);
    
    size_t size = buffer.size() + 1;
    char *data = new char[size];
    strncpy(data, buffer.c_str(), size);

    cout << "String original: " << data << endl;

    uint8_t check8 = checksum::check8(data, size);
    cout << "Checksum 8 bits = " << checksum::isValid(data, size, check8) << endl;

    uint16_t check16 = checksum::check16(data, size);
    cout << "Checksum 16 bits = " << checksum::isValid(data, size, check16) << endl;

    char *conv = convolution::code(data, size);
    cout << "Codigo convolucional = " << conv << endl;

    char *flipped = bits::flip(conv, size * 2);
    cout << "Codigo convolucional com bits flippados = " << flipped << endl;

    char *conv_dec = convolution::viterbiDecoder(flipped, size * 2);
    cout << "Decodifica o codigo convolucional = " << conv_dec << endl;

    delete[] conv;
    delete[] conv_dec;
    delete[] flipped;
    delete[] data;

    return 0;
}
