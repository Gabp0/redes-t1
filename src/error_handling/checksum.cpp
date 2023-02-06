#include "errors.h"
#include <iostream>

using namespace std;

uint8_t errors::checksum8(char *data, size_t size)
// checksum de 8 bits
{
    uint16_t sum = 0;

    for (size_t i = 0; i < size; i++)
    {
        sum += ((uint8_t)data[i]);

        // soma o carry
        if (sum >> 8)
        {
            sum = (sum & 0x00ff) + 1;
        }
    }

    return ~sum;
}

uint16_t errors::checksum16(char *data, size_t size)
// checksum de 16 bits
{
    uint32_t sum = data[0];

    size_t s = 1;
    if ((size % 2) == 0)
    {
        sum += (uint8_t)data[1];
        s = 2;
    }

    for (size_t i = s; i < size; i += 2)
    {
        uint16_t cbytes = (((uint8_t)data[i]) << 8) | ((uint8_t)data[i + 1]);
        sum += cbytes;

        // soma o carry
        if (sum >> 16)
        {
            sum = (sum & 0x0000ffff) + 1;
        }
    }

    return ~sum;
}