#include "errors.h"
#include <iostream>

using namespace std;

uint8_t errors::checksum8(char *data, size_t size)
// checksum de 8 bits
{
    uint16_t sum = 0;

    for (size_t i = 0; i < size; i++)
    {
        sum += (uint8_t)data[i];

        // soma o carry
        if (sum >> 8)
        {
            sum &= 0x00ff;
            sum += 1;
        }
    }

    sum = ~sum;

    return sum;
}

uint16_t errors::checksum16(char *data, size_t size)
// checksum de 16 bits
{
    uint32_t sum = 0;

    for (size_t i = 0; i < size; i += 2)
    {
        uint16_t cbytes = data[i];
        cbytes <<= 8;

        if (data[i + 1])
        {
            uint8_t nbyte = data[i + 1];
            cbytes |= nbyte;
        }

        sum += cbytes;

        // soma o carry
        if (sum >> 16)
        {
            sum &= 0x0000ffff;
            sum += 1;
        }
    }

    sum = ~sum;

    return sum;
}