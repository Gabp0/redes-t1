#include "checksum.h"
#include "convolution.h"
#include "../utils/bits.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    char data[34] = "mano n aguento mais esse trabalho";
    size_t size = 34;

    uint8_t check8 = checksum::check8(data, size);
    printf("Check 8 bits = %d\n", checksum::isValid(data, size, check8));

    uint16_t check16 = checksum::check16(data, size);
    printf("Check 16 bits = %d\n", checksum::isValid(data, size, check16));

    char *conv = convolution::code(data, size);
    printf("Check convolution = %s\n", conv);

    char *flipped = bits::flip(conv, size * 2);
    printf("flipado: %s\n", flipped);

    char *conv_dec = convolution::viterbiDecoder(flipped, size * 2);
    printf("Check convolution decoding = %s\n", conv_dec);

    delete[] conv;
    delete[] conv_dec;
    delete[] flipped;

    return 0;
}
