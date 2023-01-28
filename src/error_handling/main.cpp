#include "checksum.h"
#include "convolution.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    char data[17] = "mano socorro pvf";
    size_t size = 17;

    uint8_t check8 = checksum::check8(data, size);
    printf("Check 8 bits = %d\n", checksum::isValid(data, size, check8));

    uint16_t check16 = checksum::check16(data, size);
    printf("Check 16 bits = %d\n", checksum::isValid(data, size, check16));

    char *conv = convolution::code(data, size);
    printf("Check convolution = %s\n", conv);

    char *conv_dec = convolution::viterbiDecoder(data, size);
    printf("Check convolution decoding = %s\n", conv_dec);

    return 0;
}
