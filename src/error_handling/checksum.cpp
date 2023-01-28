#include "checksum.h"
#include <iostream>

using namespace std;

uint8_t checksum::check8(char *data, size_t size)
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

uint16_t checksum::check16(char *data, size_t size)
{
    uint32_t sum = 0;

    for (size_t i = 0; i < size; i += 2)
    {
        uint16_t cbytes = (uint8_t)data[i];

        if (data[i + 1])
        {
            cbytes = (cbytes << 8) | ((uint8_t)data[i]);
        }
        else
        {
            cbytes = (cbytes << 8);
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

bool checksum::isValid(char *data, size_t size, uint8_t check)
{
    return (check8(data, size) == check);
}

bool checksum::isValid(char *data, size_t size, uint16_t check)
{
    return (check16(data, size) == check);
}